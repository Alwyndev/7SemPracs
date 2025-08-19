#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_NONTERMS 32
#define MAX_PRODS    256
#define MAX_RHS_SYMS 32
#define MAX_SYM_LEN  32
#define MAX_SET      256

typedef struct {
    char lhs[MAX_SYM_LEN];
    int rhsLen;
    char rhs[MAX_RHS_SYMS][MAX_SYM_LEN];
} Production;

typedef struct {
    char nt[MAX_SYM_LEN];
    int count;
    char items[MAX_SET][MAX_SYM_LEN];
} Set;

static Production prods[MAX_PRODS];
static int PROD_COUNT=0;
static char nonterms[MAX_NONTERMS][MAX_SYM_LEN];
static int NONTERM_COUNT=0;
static char startSymbol[MAX_SYM_LEN];

static Set firstSets[MAX_NONTERMS];
static Set followSets[MAX_NONTERMS];

static void trim(char *s){
    int n=(int)strlen(s);
    while(n>0 && (s[n-1]=='\n'||s[n-1]=='\r'||isspace((unsigned char)s[n-1]))) s[--n]=0;
    char *p=s;
    while(*p && isspace((unsigned char)*p)) p++;
    if(p!=s) memmove(s,p,strlen(p)+1);
}

static bool isNonTerm(const char *sym){
    for(int i=0;i<NONTERM_COUNT;i++)
        if(strcmp(nonterms[i], sym)==0) return true;
    return false;
}

static bool symbolIsEps(const char *s){ return strcmp(s,"eps")==0; }

static bool inSet(Set *s, const char *sym){
    for(int i=0;i<s->count;i++)
        if(strcmp(s->items[i], sym)==0) return true;
    return false;
}

static bool add(Set *s, const char *sym){
    if(inSet(s,sym)) return false;
    strncpy(s->items[s->count], sym, MAX_SYM_LEN-1);
    s->items[s->count][MAX_SYM_LEN-1]=0;
    s->count++;
    return true;
}

static Set* getSet(Set *arr, const char *nt){
    for(int i=0;i<NONTERM_COUNT;i++)
        if(strcmp(arr[i].nt, nt)==0) return &arr[i];
    return NULL;
}

static void addProduction(const char *lhs, char tokens[][MAX_SYM_LEN], int count){
    if(PROD_COUNT>=MAX_PRODS) return;
    strncpy(prods[PROD_COUNT].lhs,lhs,MAX_SYM_LEN-1);
    prods[PROD_COUNT].lhs[MAX_SYM_LEN-1]=0;
    prods[PROD_COUNT].rhsLen=count;
    for(int i=0;i<count;i++){
        strncpy(prods[PROD_COUNT].rhs[i], tokens[i], MAX_SYM_LEN-1);
        prods[PROD_COUNT].rhs[i][MAX_SYM_LEN-1]=0;
    }
    PROD_COUNT++;
}

static void parseProductionLine(char *line){
    trim(line);
    if(!*line) return;
    char *arrow = strstr(line,"->");
    if(!arrow){ printf("[Malformed production]\n"); return; }
    char lhsPart[256]; strncpy(lhsPart,line, arrow-line); lhsPart[arrow-line]=0; trim(lhsPart);
    if(!isNonTerm(lhsPart)){ printf("[Unknown LHS '%s']\n", lhsPart); return; }
    char buffer[1024]; strncpy(buffer, arrow+2, sizeof(buffer)-1); buffer[sizeof(buffer)-1]=0;
    char *alt = strtok(buffer,"|");
    while(alt){
        char altCopy[1024]; strncpy(altCopy, alt, sizeof(altCopy)-1); altCopy[sizeof(altCopy)-1]=0;
        trim(altCopy);
        char tokens[MAX_RHS_SYMS][MAX_SYM_LEN];
        int tcount=0;
        if(!*altCopy){ strcpy(tokens[tcount++],"eps"); }
        else {
            char *tok=strtok(altCopy," \t");
            while(tok){
                if(tcount<MAX_RHS_SYMS){
                    strncpy(tokens[tcount], tok, MAX_SYM_LEN-1);
                    tokens[tcount][MAX_SYM_LEN-1]=0;
                    tcount++;
                }
                tok=strtok(NULL," \t");
            }
            if(tcount==0) strcpy(tokens[tcount++],"eps");
        }
        addProduction(lhsPart, tokens, tcount);
        alt = strtok(NULL,"|");
    }
}

static void computeFirst(){
    for(int i=0;i<NONTERM_COUNT;i++){
        strncpy(firstSets[i].nt, nonterms[i], MAX_SYM_LEN-1);
        firstSets[i].nt[MAX_SYM_LEN-1]=0;
        firstSets[i].count=0;
    }
    bool changed;
    do{
        changed=false;
        for(int p=0;p<PROD_COUNT;p++){
            Production *pr=&prods[p];
            Set *FS_A = getSet(firstSets, pr->lhs);
            bool allNullable=true;
            for(int i=0;i<pr->rhsLen;i++){
                const char *sym=pr->rhs[i];
                if(symbolIsEps(sym)){
                    if(add(FS_A,"eps")) changed=true;
                    break;
                }
                if(isNonTerm(sym)){
                    Set *FS_B = getSet(firstSets, sym);
                    for(int k=0;k<FS_B->count;k++)
                        if(!symbolIsEps(FS_B->items[k]))
                            if(add(FS_A, FS_B->items[k])) changed=true;
                    if(!inSet(FS_B,"eps")){
                        allNullable=false;
                        goto seq_done;
                    }
                } else {
                    if(add(FS_A, sym)) changed=true;
                    allNullable=false;
                    goto seq_done;
                }
            }
seq_done:
            if(allNullable) if(add(FS_A,"eps")) changed=true;
        }
    } while(changed);
}

static int productionsAddedSoFar(void){ return PROD_COUNT; }

int main(void){
    setvbuf(stdout,NULL,_IONBF,0);
    printf("Enter number of non-terminals: "); fflush(stdout);
    scanf("%d",&NONTERM_COUNT);
    if(NONTERM_COUNT>MAX_NONTERMS) NONTERM_COUNT=MAX_NONTERMS;
    printf("Enter non-terminals (tokens) separated by space:\n"); fflush(stdout);
    for(int i=0;i<NONTERM_COUNT;i++) scanf("%31s", nonterms[i]);
    printf("Enter start symbol: "); fflush(stdout);
    scanf("%31s", startSymbol);
    int nLines;
    printf("Enter number of production lines: "); fflush(stdout);
    scanf("%d",&nLines);
    getchar();
    printf("Enter productions (A -> alpha | beta ... , eps for epsilon):\n"); fflush(stdout);

    int accepted=0;
    while(accepted<nLines){
        char line[1024];
        if(!fgets(line,sizeof(line), stdin)) continue;
        char tmp[1024]; strcpy(tmp,line); trim(tmp);
        if(!*tmp){ printf("[Ignored blank line]\n"); continue; }
        int before=productionsAddedSoFar();
        parseProductionLine(line);
        if(productionsAddedSoFar()==before){
            printf("[No production parsed, retry]\n");
            continue;
        }
        accepted++;
    }

    computeFirst();

    for(int i=0;i<NONTERM_COUNT;i++){
        strncpy(followSets[i].nt, nonterms[i], MAX_SYM_LEN-1);
        followSets[i].nt[MAX_SYM_LEN-1]=0;
        followSets[i].count=0;
    }
    add(getSet(followSets, startSymbol), "$");

    bool changed;
    do{
        changed=false;
        for(int p=0;p<PROD_COUNT;p++){
            Production *pr=&prods[p];
            for(int i=0;i<pr->rhsLen;i++){
                const char *B = pr->rhs[i];
                if(symbolIsEps(B) || !isNonTerm(B)) continue;
                bool betaNullable=true;
                for(int j=i+1;j<pr->rhsLen;j++){
                    const char *sym = pr->rhs[j];
                    if(symbolIsEps(sym)){
                        // continue
                    } else if(isNonTerm(sym)){
                        Set *FS = getSet(firstSets, sym);
                        for(int k=0;k<FS->count;k++)
                            if(!symbolIsEps(FS->items[k]))
                                if(add(getSet(followSets,B), FS->items[k])) changed=true;
                        if(!inSet(FS,"eps")){
                            betaNullable=false;
                            break;
                        }
                    } else {
                        if(add(getSet(followSets,B), sym)) changed=true;
                        betaNullable=false;
                        break;
                    }
                }
                if(i==pr->rhsLen-1 || betaNullable){
                    Set *FA = getSet(followSets, pr->lhs);
                    Set *FB = getSet(followSets, B);
                    for(int t=0;t<FA->count;t++)
                        if(add(FB, FA->items[t])) changed=true;
                }
            }
        }
    } while(changed);

    printf("\nFIRST sets:\n");
    for(int i=0;i<NONTERM_COUNT;i++){
        Set *s=&firstSets[i];
        printf("FIRST(%s) = { ", s->nt);
        for(int j=0;j<s->count;j++)
            printf("%s%s", s->items[j], (j+1<s->count)?", ":"");
        printf(" }\n");
    }
    printf("\nFOLLOW sets:\n");
    for(int i=0;i<NONTERM_COUNT;i++){
        Set *s=&followSets[i];
        printf("FOLLOW(%s) = { ", s->nt);
        for(int j=0;j<s->count;j++)
            printf("%s%s", s->items[j], (j+1<s->count)?", ":"");
        printf(" }\n");
    }
    return 0;
}
