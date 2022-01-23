#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXCHAR 1000
#define MAXTAG 100

enum tagList{html, _html, head, _head, body, _body, title, _title, h1, _h1, h2, _h2, h3, _h3, p, _p, ul, _ul, li, _li, a, _a, Div, _Div, br, hr, dummy, error};

int top = 0;

void append(int list[], int num) {
    int i = 0;
    while (list[i] != -1){
        i++;
    }
    list[i] = num;
}

int removeFirst(int list[]) {
    int first = list[0];
    for (int i = 1; i < MAXTAG; i++){
        list[i-1] = list[i];
    }
    return first;
}

void push(int stack[], int num) {
    stack[top] = num;
    top++;
}

int pop(int stack[]) {
    int pop = stack[top-1];
    stack[top-1] = 99;
    top--;
    return pop;
}

int tagToToken(char* tag) {
    int token;

    if (strcmp(tag, "html") == 0){
        token = html;
    }
    else if (strcmp(tag, "head") == 0){
        token = head;
    }
    else if (strcmp(tag, "body") == 0){
        token = body;
    }
    else if (strcmp(tag, "title") == 0){
        token = title;
    }
    else if (strcmp(tag, "h1") == 0){
        token = h1;
    }
    else if (strcmp(tag, "h2") == 0){
        token = h2;
    }
    else if (strcmp(tag, "h3") == 0){
        token = h3;
    }
    else if (strcmp(tag, "p") == 0){
        token = p;
    }
    else if (strcmp(tag, "ul") == 0){
        token = ul;
    }
    else if (strcmp(tag, "li") == 0){
        token = li;
    }
    else if (strcmp(tag, "a") == 0){
        token = a;
    }
    else if (strcmp(tag, "div") == 0){
        token = Div;
    }
    else if (strcmp(tag, "br") == 0){
        token = br;
    }
    else if (strcmp(tag, "hr") == 0){
        token = hr;
    }
    else{
        token = error;
    }

    return token;
}

void tokenize(char* line, int tokens[]) {
    char *head = line;
    char *endOfLine = strchr(line, '\0');
    int lineLen = endOfLine - head;
    int charCount;

    while (*head != '\0'){
        if (*head == '\r'|| *head == '\n'|| *head == '\t'){
            charCount = 1;
        }
        else if (*head == '<'){
            char *tail = strchr(head+1, '>');
            if (tail == NULL){
                append(tokens, error);
                break;
            }
            char *completeTag = (char*) malloc((tail - head) * sizeof(char));
            strncpy(completeTag, head+1, tail-head-1);
            completeTag[tail-head-1] = '\0';
            char *tag = strtok(completeTag, " ");
            int token;
            if (tag[0] == '/'){
                tag++;
                token = tagToToken(tag);
                if (token < br){
                    token++;
                }
                else{
                    token = error;
                }
            }
            else{
                token = tagToToken(tag);
            }
            append(tokens, token);
            charCount = tail - head + 1;
        }
        else{
            char *tail = strchr(head, '<');
            if (tail == NULL){
                tail = strchr(head, '\0');
            }
            charCount = tail - head;
        }
        lineLen -= charCount;
        if (lineLen<0){
            append(tokens, error);
            break;
        }
        head += charCount;
    } 
}

void checkPAndDiv(int current, int stack[]) {
    if (current == p || current == Div){
        int topOfStack = pop(stack);
        if (topOfStack == p){
            printf("Error: A <p> or <div> is nested inside a <p>.");
            exit(0);
        }
        else{
            push(stack, topOfStack);
        }    
    } 
}

void checkNestedToken(int current, int stack[]) {
    int type = current % 2;
    if (current == error) {
        printf("Error: There is an invalid tag.");
        exit(0);
    }
    else if (current >= html && current <= _Div) {
        if (type == 0){
            checkPAndDiv(current, stack);
            push(stack, current);
        }
        else if (type == 1){
            int topOfStack = pop(stack);
            if (current-1 != topOfStack)
            {
                printf("Error: There is an unmatched tag.");
                exit(0);
            }
        }
    }
}

void checkHtml(int tokens[], int stack[]) {
    int current = removeFirst(tokens);
    if (current != html) {
        printf("Error: The first tag should be <html>.");
        exit(0);
    }
    else{
        push(stack, html);
    }
}

void checkHead(int tokens[], int stack[]) {
    int current = removeFirst(tokens);
    if (current != head) {
        printf("Error: The <head> part is missing.");
        exit(0);
    }
    else{
        push(stack, head);
    }
}

void checkInsideHead(int tokens[], int stack[]) {
    int current = removeFirst(tokens);
    while (current != _head){
        checkNestedToken(current, stack);
        current = removeFirst(tokens);
    }
}

void checkCloseHead(int tokens[], int stack[]) {
    int topOfStack = pop(stack);
    if (topOfStack != head){
        printf("Error: There is an unmatched tag.");
        exit(0);
    }
}

void checkBody(int tokens[], int stack[]) {
    int current = removeFirst(tokens);
    if (current != body){
        printf("Error: The <body> part is missing.");
        exit(0);
    }
    else{
        push(stack, current);
    }
}

void checkInsideBody(int tokens[], int stack[]) {
    int current = removeFirst(tokens);
    while (current != _body){
        if (current == title){
            printf("Error: <title> should be in the head section only.");
            exit(0);
        }
        else{
            checkNestedToken(current, stack);
        }
        current = removeFirst(tokens);
    }
}

void checkCloseBody(int tokens[], int stack[]) {
    int topOfStack = pop(stack);
    if (topOfStack != body){
        printf("Error: There is an unmatched tag.");
        exit(0);
    }
}

void checkCloseHtml(int tokens[], int stack[]) {
    int topOfStack_ = pop(stack);
    int current = removeFirst(tokens);
    if (topOfStack_ != html){
        printf("Error: There is an unmatched tag.");
        exit(0);
    }
    else if (current != _html){
        printf("Error: There is no closing tag for <html>.");
        exit(0);
    }
}

void checkToken(int tokens[]) {
    int stack[MAXTAG];
    for (int i = 0; i < MAXTAG; i++){
        stack[i] = 99;
    }

    checkHtml(tokens, stack);
    checkHead(tokens, stack);
    checkInsideHead(tokens, stack);
    checkCloseHead(tokens, stack);
    checkBody(tokens, stack);
    checkInsideBody(tokens, stack);
    checkCloseBody(tokens, stack);
    checkCloseHtml(tokens, stack);
    
    printf("This is a valid HTML file.");
}

int main() {
    FILE *fp;
    fp = fopen("file.html", "r");
    char line[MAXCHAR];
    int tokens[MAXTAG];
    for (int i = 0; i < MAXTAG; i++){
        tokens[i] = -1;
    }

    while (fgets(line, MAXCHAR, fp) != NULL) {
        tokenize(line, tokens);
    }
    fclose(fp);

    checkToken(tokens);
    return 0;
}