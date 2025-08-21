import re

EPS_SYMS = {"ε", "Є", "eps", "EPS", "Ep", "e"}

def normalize_epsilon(tok: str) -> str:
    return "ε" if tok in EPS_SYMS else tok

def normalize_apostrophes(s: str) -> str:
    return s.replace("'", "'").replace("'", "'")

class ParseTree:
    def __init__(self, symbol):
        self.symbol = symbol
        self.children = []
    
    def add_child(self, child):
        self.children.append(child)
    
    def print_tree(self, indent=0):
        print("  " * indent + self.symbol)
        for child in self.children:
            if isinstance(child, ParseTree):
                child.print_tree(indent + 1)

class PredictiveParser:
    def __init__(self, grammar):
        self.grammar = grammar
        self.parsing_table = {}
        self.build_parsing_table()
    
    def build_parsing_table(self):
        """Build LL(1) parsing table using FIRST and FOLLOW sets"""
        # Initialize table
        for nt in self.grammar.non_terminals:
            self.parsing_table[nt] = {}
        
        # For each production A -> α
        for lhs, rhs in self.grammar.productions:
            # Get FIRST(α)
            first_alpha = self.grammar.first_of_sequence(rhs)
            
            # For each terminal a in FIRST(α)
            for terminal in first_alpha:
                if terminal != "ε":
                    if terminal not in self.parsing_table[lhs]:
                        self.parsing_table[lhs][terminal] = (lhs, rhs)
                    else:
                        print(f"Conflict at [{lhs}, {terminal}] - Grammar not LL(1)")
            
            # If ε in FIRST(α), add A -> α to M[A, b] for each b in FOLLOW(A)
            if "ε" in first_alpha:
                for terminal in self.grammar.follow_sets[lhs]:
                    if terminal not in self.parsing_table[lhs]:
                        self.parsing_table[lhs][terminal] = (lhs, rhs)
                    else:
                        print(f"Conflict at [{lhs}, {terminal}] - Grammar not LL(1)")
    
    def print_parsing_table(self):
        """Print the parsing table"""
        print("\nParsing Table:")
        print("-" * 50)
        
        # Get all terminals that appear in the table
        all_terminals = set()
        for nt_dict in self.parsing_table.values():
            all_terminals.update(nt_dict.keys())
        all_terminals = sorted(all_terminals)
        
        # Print header
        print(f"{'NT':<8}", end="")
        for term in all_terminals:
            print(f"{term:<12}", end="")
        print()
        print("-" * (8 + 12 * len(all_terminals)))
        
        # Print table rows
        for nt in self.grammar.non_terminals:
            print(f"{nt:<8}", end="")
            for term in all_terminals:
                if term in self.parsing_table[nt]:
                    lhs, rhs = self.parsing_table[nt][term]
                    prod = f"{lhs}→{''.join(rhs)}"
                    print(f"{prod:<12}", end="")
                else:
                    print(f"{'--':<12}", end="")
            print()
    
    def parse(self, input_string):
        """Parse input string and return parse tree"""
        tokens = self.grammar.tokenize_rhs_part(input_string)
        tokens.append("$")  # End of input marker
        
        stack = ["$", self.grammar.start_symbol]
        parse_tree = ParseTree(self.grammar.start_symbol)
        node_stack = [None, parse_tree]  # Stack to track tree nodes
        
        input_ptr = 0
        
        print(f"\nParsing: {' '.join(tokens[:-1])}")
        print(f"{'Step':<5} {'Stack':<25} {'Input':<20} {'Action'}")
        print("-" * 70)
        
        step = 1
        while len(stack) > 1:
            top = stack[-1]
            current_input = tokens[input_ptr] if input_ptr < len(tokens) else "$"
            current_node = node_stack[-1]
            
            # Display current state
            stack_str = ' '.join(stack)
            input_str = ' '.join(tokens[input_ptr:])
            print(f"{step:<5} {stack_str:<25} {input_str:<20}", end=" ")
            
            if top == current_input:
                # Match terminal
                stack.pop()
                node_stack.pop()
                input_ptr += 1
                print(f"Match {top}")
            elif self.grammar.is_non_terminal(top):
                # Apply production
                if top in self.parsing_table and current_input in self.parsing_table[top]:
                    lhs, rhs = self.parsing_table[top][current_input]
                    stack.pop()
                    node_stack.pop()
                    
                    # Add children to parse tree
                    if rhs != ["ε"]:
                        # Add symbols to stack in reverse order
                        for symbol in reversed(rhs):
                            stack.append(symbol)
                        
                        # Add children to tree in correct order
                        for symbol in rhs:
                            child_node = ParseTree(symbol)
                            current_node.add_child(child_node)
                            node_stack.append(child_node)
                    else:
                        # Epsilon production
                        epsilon_node = ParseTree("ε")
                        current_node.add_child(epsilon_node)
                    
                    print(f"Apply {lhs} → {''.join(rhs)}")
                else:
                    print(f"ERROR: No rule for [{top}, {current_input}]")
                    return None
            else:
                print(f"ERROR: Expected {top}, got {current_input}")
                return None
            
            step += 1
        
        if input_ptr == len(tokens) - 1 and tokens[input_ptr] == "$":
            print("Parsing successful!")
            return parse_tree
        else:
            print("Parsing failed!")
            return None

class Grammar:
    def __init__(self, non_terminals, start_symbol):
        self.non_terminals = non_terminals
        self.start_symbol = start_symbol
        self.productions = []  # list of (lhs, rhs_list)
        self.first_sets = {nt: set() for nt in non_terminals}
        self.follow_sets = {nt: set() for nt in non_terminals}
        self.follow_sets[start_symbol].add("$")
        self.original_lines = []  # keep raw production lines (in input order)

    def is_non_terminal(self, sym):
        return sym in self.non_terminals

    def add_production(self, lhs, rhs_symbols):
        # rhs_symbols == ['ε'] for epsilon production
        self.productions.append((lhs, rhs_symbols))

    # REPLACED tokenize_rhs_part: always regex-scan (handles compact forms like +T, *F, TE')
    def tokenize_rhs_part(self, text):
        text = normalize_apostrophes(text)
        pattern = re.compile(r"id|[A-Za-z](?:'+)?|ε|Є|\+|-|\*|/|\(|\)")
        tokens = []
        i = 0
        while i < len(text):
            ch = text[i]
            if ch.isspace():
                i += 1
                continue
            m = pattern.match(text, i)
            if m:
                tok = normalize_epsilon(m.group(0))
                tokens.append(tok)
                i = m.end()
            else:
                # Fallback: single char token
                tokens.append(text[i])
                i += 1
        if not tokens:
            return ["ε"]
        return tokens

    def parse_production_line(self, line):
        raw = line.rstrip("\n")
        line = normalize_apostrophes(raw.strip())
        if not line or "->" not in line:
            return
        lhs, rhs = line.split("->", 1)
        lhs = lhs.strip()
        if not self.is_non_terminal(lhs):
            return
        if raw and raw not in self.original_lines:
            self.original_lines.append(raw)
        for alt in rhs.split("|"):
            alt = alt.strip()
            if not alt:
                self.add_production(lhs, ["ε"])
            else:
                tokens = self.tokenize_rhs_part(alt)
                self.add_production(lhs, tokens)

    def compute_first(self):
        changed = True
        while changed:
            changed = False
            for lhs, rhs in self.productions:
                # epsilon production
                if rhs == ["ε"]:
                    if "ε" not in self.first_sets[lhs]:
                        self.first_sets[lhs].add("ε")
                        changed = True
                    continue
                nullable_prefix = True
                for sym in rhs:
                    if sym == "ε":
                        if "ε" not in self.first_sets[lhs]:
                            self.first_sets[lhs].add("ε")
                            changed = True
                        nullable_prefix = False
                        break
                    if self.is_non_terminal(sym):
                        # add FIRST(sym) - {ε}
                        before = len(self.first_sets[lhs])
                        self.first_sets[lhs].update(self.first_sets[sym] - {"ε"})
                        if len(self.first_sets[lhs]) > before:
                            changed = True
                        if "ε" in self.first_sets[sym]:
                            # continue to next symbol
                            pass
                        else:
                            nullable_prefix = False
                            break
                    else:
                        # terminal
                        if sym not in self.first_sets[lhs]:
                            self.first_sets[lhs].add(sym)
                            changed = True
                        nullable_prefix = False
                        break
                if nullable_prefix:
                    if "ε" not in self.first_sets[lhs]:
                        self.first_sets[lhs].add("ε")
                        changed = True

    def first_of_sequence(self, seq):
        result = set()
        if not seq:
            result.add("ε")
            return result
        for sym in seq:
            if sym == "ε":
                result.add("ε")
                break
            if self.is_non_terminal(sym):
                result.update(self.first_sets[sym] - {"ε"})
                if "ε" in self.first_sets[sym]:
                    continue
                else:
                    break
            else:
                result.add(sym)
                break
        else:
            result.add("ε")
        return result

    def compute_follow(self):
        changed = True
        while changed:
            changed = False
            for lhs, rhs in self.productions:
                for i, B in enumerate(rhs):
                    if not self.is_non_terminal(B):
                        continue
                    beta = rhs[i+1:]
                    first_beta = self.first_of_sequence(beta)
                    # FIRST(β) - {ε}
                    before = len(self.follow_sets[B])
                    self.follow_sets[B].update(first_beta - {"ε"})
                    if len(self.follow_sets[B]) > before:
                        changed = True
                    # if β nullable add FOLLOW(lhs)
                    if "ε" in first_beta or not beta:
                        before2 = len(self.follow_sets[B])
                        self.follow_sets[B].update(self.follow_sets[lhs])
                        if len(self.follow_sets[B]) > before2:
                            changed = True

    def compute(self):
        self.compute_first()
        self.compute_follow()

    def create_parser(self):
        """Create a predictive parser for this grammar"""
        return PredictiveParser(self)

    def print_first_sets(self):
        def disp_nt(nt): return nt.replace("'", "'")
        def disp_tok(t):
            if self.is_non_terminal(t) and t != "ε":  # skip non-terminals inside FIRST
                return None
            return "Є" if t == "ε" else t
        # Build normalized display sets
        groups = []
        seen = set()
        for nt in self.non_terminals:
            if nt in seen: continue
            base_items = {disp_tok(t) for t in self.first_sets[nt]}
            base_items.discard(None)
            group = [nt]
            for other in self.non_terminals:
                if other == nt or other in seen: continue
                other_items = {disp_tok(t) for t in self.first_sets[other]}
                other_items.discard(None)
                if other_items == base_items:
                    group.append(other)
            for gnt in group: seen.add(gnt)
            groups.append((group, base_items))
        print("\nFIRST set")
        for group, items in groups:
            lhs_chain = " = ".join(f"FIRST({disp_nt(g)})" for g in group)
            ordered = sorted(items)
            print(f"{lhs_chain} = {{ " + " , ".join(ordered) + " }}")

    def print_follow_sets(self):
        def disp_nt(nt): return nt.replace("'", "'")
        def disp_tok(t):
            if self.is_non_terminal(t) and t != "ε":
                return None
            return "Є" if t == "ε" else t
        groups = []
        seen = set()
        for nt in self.non_terminals:
            if nt in seen: continue
            base_items = {disp_tok(t) for t in self.follow_sets[nt]}
            base_items.discard(None)
            group = [nt]
            for other in self.non_terminals:
                if other == nt or other in seen: continue
                other_items = {disp_tok(t) for t in self.follow_sets[other]}
                other_items.discard(None)
                if other_items == base_items:
                    group.append(other)
            for gnt in group: seen.add(gnt)
            groups.append((group, base_items))
        print("\nFOLLOW Set")
        for group, items in groups:
            lhs_chain = " = ".join(f"FOLLOW({disp_nt(g)})" for g in group)
            ordered = sorted(items)
            print(f"{lhs_chain} = {{ " + " , ".join(ordered) + " }}")

def main():
    n = int(input("Enter the number of non-terminals  :  ").strip())
    non_terms = input("Enter the non-terminals separated by space: ").strip()
    non_terminals = [normalize_apostrophes(t) for t in non_terms.split()]
    if len(non_terminals) != n:
        print(f"[Warning] Expected {n} non-terminals, got {len(non_terminals)}")
    start_symbol = normalize_apostrophes(input("Enter the start symbol :  ").strip())
    grammar = Grammar(non_terminals, start_symbol)
    print("Enter the productions in the format 'A -> a | b':")
    
    lines_read = 0
    while True:
        try:
            line = input()
        except EOFError:
            break
        if not line.strip():
            break
        grammar.parse_production_line(line)
        lines_read += 1
    
    grammar.compute()
    
    # Echo grammar (original lines as entered)
    print()
    for line in grammar.original_lines:
        print(line)
    
    grammar.print_first_sets()
    grammar.print_follow_sets()
    
    # Create parser and show parsing table
    parser = grammar.create_parser()
    parser.print_parsing_table()
    
    # Parse input strings
    while True:
        try:
            input_str = input("\nEnter string to parse (or press Enter to quit): ").strip()
            if not input_str:
                break
            
            parse_tree = parser.parse(input_str)
            if parse_tree:
                print("\nParse Tree:")
                parse_tree.print_tree()
        except EOFError:
            break

if __name__ == "__main__":
    main()