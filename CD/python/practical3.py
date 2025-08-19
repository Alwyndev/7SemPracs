import re

EPS_SYMS = {"ε", "Є", "eps", "EPS", "Ep", "e"}

def normalize_epsilon(tok: str) -> str:
    return "ε" if tok in EPS_SYMS else tok

def normalize_apostrophes(s: str) -> str:
    return s.replace("’", "'").replace("‘", "'")

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

    def print_first_sets(self):
        def disp_nt(nt): return nt.replace("'", "’")
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
        def disp_nt(nt): return nt.replace("'", "’")
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
    # Read until blank line or until at least one production per non-terminal
    lines_read = 0
    # We cannot rely strictly on n lines because some lines may contain multiple alternatives.
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

if __name__ == "__main__":
    main()