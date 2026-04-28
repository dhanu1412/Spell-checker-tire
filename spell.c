#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define MAX_LEN 100
#define MAX_WORDS 250000 

struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
};

char (*dictList)[MAX_LEN]; 
int totalWords = 0;

struct TrieNode *getNode(void) {
    struct TrieNode *pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (pNode) {
        pNode->isEndOfWord = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++)
            pNode->children[i] = NULL;
    }
    return pNode;
}

void strictClean(char *word) {
    int j = 0;
    char temp[MAX_LEN];
    for (int i = 0; word[i] != '\0'; i++) {
        if (isalpha((unsigned char)word[i])) {
            temp[j++] = tolower((unsigned char)word[i]);
        }
    }
    temp[j] = '\0';
    strcpy(word, temp);
}

void insert(struct TrieNode *root, const char *key) {
    struct TrieNode *pCrawl = root;
    for (int i = 0; key[i] != '\0'; i++) {
        int index = key[i] - 'a';
        if (index < 0 || index >= 26) continue;
        if (!pCrawl->children[index])
            pCrawl->children[index] = getNode();
        pCrawl = pCrawl->children[index];
    }
    pCrawl->isEndOfWord = 1;
}

int search(struct TrieNode *root, const char *key) {
    struct TrieNode *pCrawl = root;
    for (int i = 0; key[i] != '\0'; i++) {
        int index = key[i] - 'a';
        if (index < 0 || index >= 26) return 0;
        if (!pCrawl->children[index])
            return 0;
        pCrawl = pCrawl->children[index];
    }
    return (pCrawl != NULL && pCrawl->isEndOfWord);
}

int minVal(int a, int b, int c) {
    if (a <= b && a <= c) return a;
    if (b <= a && b <= c) return b;
    return c;
}

int getEditDistance(char *s1, char *s2) {
    int m = strlen(s1);
    int n = strlen(s2);
    int dp[m + 1][n + 1];

    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0) dp[i][j] = j;
            else if (j == 0) dp[i][j] = i;
            else if (s1[i - 1] == s2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = 1 + minVal(dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]);
        }
    }
    return dp[m][n];
}

void findSuggestions(char *word) {
    int displayedCount = 0;
    printf("Suggestions: ");
    
    for (int i = 0; i < totalWords; i++) {
        if (abs((int)strlen(dictList[i]) - (int)strlen(word)) > 1) continue;
        if (getEditDistance(word, dictList[i]) == 1) {
            printf("%s ", dictList[i]);
            displayedCount++;
        }
        if (displayedCount >= 5) break; 
    }

    if (displayedCount == 0)
        printf("No suggestions");

    printf("\n");
}

void loadDictionary(struct TrieNode *root, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error loading dictionary\n");
        exit(1);
    }

    dictList = malloc(MAX_WORDS * sizeof(*dictList));

    char buffer[MAX_LEN];
    while (fgets(buffer, MAX_LEN, file) && totalWords < MAX_WORDS) {
        strictClean(buffer);
        if (strlen(buffer) > 0) {
            insert(root, buffer);
            strcpy(dictList[totalWords++], buffer);
        }
    }
    fclose(file);
}

int main() {
    struct TrieNode *root = getNode();
    
    printf("Loading dictionary...\n");
    loadDictionary(root, "words.txt");

    char line[500];
    printf("Enter a sentence to check: "); 
    
    if (fgets(line, sizeof(line), stdin)) {
        char *token = strtok(line, " \n\r"); 
        while (token != NULL) {
            char word[MAX_LEN];
            strcpy(word, token);
            strictClean(word);

            if (strlen(word) > 0) {
                if (search(root, word)) {
                    printf("[ %s ] : Correct\n", word);
                } else {
                    printf("[ %s ] : Incorrect\n", word);
                    findSuggestions(word);
                }
            }
            token = strtok(NULL, " \n\r");
        }
    }

    return 0;
}