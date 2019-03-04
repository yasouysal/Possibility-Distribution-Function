#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

struct dugum {
    char simge;
    struct dugum* ptr;
};

struct variableLine{
    char variableName;
    float lower;
    float upper;
    float* probabilityArr;
};

struct evaluation{
    double value;
    struct evaluation* ptr2;
    
};

void push2(struct evaluation **p, double value){
    struct evaluation* yeniptr;
    yeniptr = malloc(sizeof(struct evaluation));
    if (yeniptr != NULL) {
        yeniptr->value = value;
        yeniptr->ptr2 = *p;
        *p = yeniptr;
    }
}

double pop2(struct evaluation **p){
    struct evaluation * geciciptr;
    double value;

    geciciptr = *p;
    value = (*p)->value;
    *p = (*p)->ptr2;
    free(geciciptr);
    return value;
}


void eliminateChars(char* s, char ch){
    char *p = s; 
    while (*s) {
        if (*s != ch){
            *p++ = *s;

        }
        s++;
    }
    *p = 0;
}

void printCharArray(char* s){
    while (*s) {
        printf("%c", *s);
        s++;
    }
    printf("\n");

}

void push(struct dugum **p, char simge){
    struct dugum* yeniptr;
    yeniptr = malloc(sizeof(struct dugum));
    if (yeniptr != NULL) {
        yeniptr->simge = simge;
        yeniptr->ptr = *p;
        *p = yeniptr;
    }
}

char pop(struct dugum **p){
    struct dugum * geciciptr;
    char simge;

    geciciptr = *p;
    simge = (*p)->simge;
    *p = (*p)->ptr;
    free(geciciptr);
    return simge;
}


char top(struct dugum* p){
    if (p){
        return (*p).simge;
    }
    return 0;
}

double randomGenerate(char letter, struct variableLine** lineArr, int intervalCount){
    int n = 0;
    int m = 0;
    int selectedInterval = 0;
    float upperLimit;
    float lowerLimit;
    float intervalLength;
    float* probabilities;
    double stackedProbability = 0;
    double val = 0;
    double pickedNumberInInterval = 0;
    double randomizedNumber = 0;

    for (n = 0; n < 26; n++){
        if (letter == (lineArr[n]->variableName)){
            break;
        }
    }

    upperLimit = lineArr[n]->upper;
    lowerLimit = lineArr[n]->lower;
    probabilities = lineArr[n]->probabilityArr;
    stackedProbability = probabilities[0];

    intervalLength = (upperLimit - lowerLimit) / intervalCount;

    val = (rand() * 1.0) / (RAND_MAX * 1.0);

    for (m = 0; m < intervalCount; m++){
        if (val < stackedProbability){
            selectedInterval = m;
            break;
        }

        else{
            stackedProbability += probabilities[m+1];
        }
    }

    pickedNumberInInterval = ((double)rand() / RAND_MAX) * intervalLength;

    randomizedNumber = lowerLimit + (m * intervalLength) + pickedNumberInInterval;

    return randomizedNumber;

}


double evaluate(char postArrayOriginal[200], struct variableLine** lineArr, int intervalCount){
    const char s[2] = "|";
    int a = 0;
    double randomizedVariables[256];
    double randomizedVariableValue;
    char *token;
    double firstOperand, secondOperand, result;
    struct evaluation* evaluationStack;
    char postArray[200];
    strcpy(postArray, postArrayOriginal);
    for (a = 0; a < 256; a++){
        randomizedVariables[a] = 0;
    }
    token = strtok(postArray, s);
    while (token != NULL){

        if ((token[0] == '+') || (token[0] == '-') || (token[0] == '*') || (token[0] == '/') || (token[0] == '^')){
            firstOperand = pop2(&evaluationStack);
            secondOperand = pop2(&evaluationStack);
            if (token[0] == '+'){
                result = secondOperand + firstOperand;
            }

            else if (token[0] == '-'){
                result = secondOperand - firstOperand;
            }

            else if (token[0] == '*'){
                result = secondOperand * firstOperand;
            }

            else if(token[0] == '/'){
                result = secondOperand / firstOperand;
            }

            else if(token[0] == '^'){
                result = pow(secondOperand, firstOperand);
            }
            push2(&evaluationStack, result); 
            

        }

        else if ((token[0] == 's') || (token[0] == 'c') || (token[0] == 'l') || (token[0] == 'q') || (token[0] == '~')){
            firstOperand = pop2(&evaluationStack);
            if (token[0] == 's'){
                result = sin(firstOperand);
            }

            else if (token[0] == 'c'){
                result = cos(firstOperand);
            }

            else if (token[0] == 'l'){
                result = log(firstOperand);
            }

            else if (token[0] == 'q'){
                result = sqrt(firstOperand);
            }

            else if (token[0] == '~'){
                result = (-1) * firstOperand;
            }
            push2(&evaluationStack, result);
        }

        else if ((token[0] >= '0' && token[0] <= '9') || token[0] == '.'){
            push2(&evaluationStack, atof(token));
        }

        else if (token[0] >= 'A' && token[0] <= 'Z'){
            if (!randomizedVariables[token[0]]){
                randomizedVariableValue = randomGenerate(token[0], lineArr, intervalCount);
                randomizedVariables[token[0]] = randomizedVariableValue;
            }
            else{
                randomizedVariableValue = randomizedVariables[token[0]];
            }
            push2(&evaluationStack, randomizedVariableValue);
        }

        token = strtok(NULL, s);

    }

    result = pop2(&evaluationStack);
    return result;
}


int main(){
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    int m = 0;
    int h = 0;
    long int experimentCounter = 0;
    char tempSimge;
    float tempUpperLimit;
    float tempLowerLimit;
    float* tempProbabilityLine;
    int intervalCount;
    long int experimentCount;
    int variableSeen = 0;
    int formulaLength = 0;
    int variableCount = 0;
    char variables[270];
    struct dugum* stack = NULL;
    struct variableLine* lineptr;
    struct variableLine** lineArr;
    char formula[200];
    char postArray[200];
    double absMax, absMin;
    double* finalProbabilityArray;
    double intervalLengthFinal;
    double evaluatedValue = 0;
    long int confirmedExperiments = 0;

    srand(time(NULL));


    while (1) {
      
        formula[i] = getchar();
        
        if (formula[i] == '\n'){
            break;
        }
        else {
                
                i++;
        }
    }
    
    scanf(" %d %ld", &intervalCount ,&experimentCount);

    formulaLength = i;
    i = 0;


    eliminateChars(formula, ' ');


    while (formula[i] != '\n' && i < formulaLength) {
        if ((formula[i] >= '0' && formula[i] <= '9') || formula[i] == '.') {
            postArray[j] = formula[i];
            if((formula[i+1] >= '0' && formula[i+1] <= '9') || formula[i+1] == '.'){    
                j++;
            }
            else{
                postArray[j+1] = '|';
                j += 2;
            }
            i++;

        }

        else if (formula[i] >= 'A' && formula[i] <= 'Z') {
            postArray[j] = formula[i];
            variableSeen = 0;                 
            for (k = 0; k < 26; k++){
                if (variables[k] == formula[i]) {
                    variableSeen = 1;
                    break;
                }
            }
            if (!variableSeen){
                variables[variableCount] = formula[i];
                variableCount += 1;
            }
            postArray[j+1] = '|';
            i++;
            j += 2;            
        }

        else if (formula[i] == '+' ||
                 formula[i] == '-' ||
                 formula[i] == '*' ||
                 formula[i] == '/' ||
                 formula[i] == '^') {
            while ((top(stack) == '+' ||
                    top(stack) == '-' ||
                    top(stack) == '*' ||
                    top(stack) == '/' ||
                    top(stack) == '^') &&
                   ((formula[i] == '+' &&
                    (top(stack) == '+' ||
                     top(stack) == '-' ||
                     top(stack) == '*' ||
                     top(stack) == '/' ||
                     top(stack) == '^')) ||  
                     (formula[i] == '-' &&
                      (top(stack) == '+' ||
                       top(stack) == '-' ||
                       top(stack) == '*' ||
                       top(stack) == '/' ||
                       top(stack) == '^')) ||
                     (formula[i] == '*' &&
                      (top(stack) == '*' ||
                       top(stack) == '/' ||
                       top(stack) == '^')) ||
                     (formula[i] == '/' &&
                      (top(stack) == '*' ||
                       top(stack) == '/' ||
                       top(stack) == '^'))
                    )) {
                postArray[j] = pop(&stack);
                postArray[j+1] = '|';
                j += 2;                                         
            }
            push(&stack ,formula[i]);
            i++;
                                        
        }
        else if (formula[i] == '('){
            push(&stack ,formula[i]);
            i++;
        }
        else if (formula[i] == ')'){
            while (top(stack) != '('){
                postArray[j] = pop(&stack);
                postArray[j+1] = '|';
                j += 2; 
            }
            pop(&stack);
            if (top(stack) == 's' || top(stack) == 'c' || top(stack) == 'l' || top(stack) == 'q' || top(stack) == '~'){
                postArray[j] = pop(&stack);
                postArray[j+1] = '|';
                j += 2;
            }
            i++; 
            
        }
        else if (formula[i] == '~' || (formula[i] == 's' && formula[i+1] == 'i') || (formula[i] == 's' && formula[i+1] == 'q') || formula[i] == 'c' || formula[i] == 'l'){
            if (formula[i] == '~'){
                push(&stack ,'~');
                i++;
            }
            else if (formula[i] == 's' && formula[i+1] == 'i'){
                push(&stack ,'s');
                i+=3;
            }
            else if (formula[i] == 's' && formula[i+1] == 'q'){
                push(&stack ,'q');
                i+=4;
            }
            else if (formula[i] == 'l'){
                push(&stack ,'l');
                i+=2;
            }
            else{
                push(&stack ,'c');
                i+=3;
            }
        }

    }

    while (top(stack) == '+' || top(stack) == '-' || top(stack) == '*' || top(stack) == '/' || top(stack) == '^'){
        postArray[j] = pop(&stack);
        postArray[j+1] = '|';
        j += 2;
    }
    postArray[j] = '\0';

    lineArr = malloc(sizeof(struct variableLine) * variableCount);

    for (l = 0; l < variableCount; l++){
        lineptr = malloc(sizeof(struct variableLine));
        
        scanf(" %c ", &tempSimge);
        scanf(" %f %f ", &tempLowerLimit , &tempUpperLimit);

 

        tempProbabilityLine = (float*)malloc(sizeof(float)*intervalCount);
        for(h = 0; h < intervalCount-1; h++){
            scanf(" %f ", &tempProbabilityLine[h]);
        }
        scanf(" %f", &tempProbabilityLine[h]);


        lineptr->variableName = tempSimge;
        lineptr->lower = tempLowerLimit;
        lineptr->upper = tempUpperLimit;
        lineptr->probabilityArr = tempProbabilityLine;

        lineArr[l] = lineptr;

    }

    evaluatedValue = evaluate(postArray, lineArr, intervalCount);



    absMax = evaluate(postArray, lineArr, intervalCount);
    absMin = absMax;


    finalProbabilityArray = malloc(intervalCount * sizeof(double));

    for (experimentCounter = 0; experimentCounter < experimentCount; experimentCounter++){
        evaluatedValue = evaluate(postArray, lineArr, intervalCount);

        if (evaluatedValue < absMin){
            absMin = evaluatedValue;
        }
        else if (evaluatedValue > absMax){
            absMax = evaluatedValue;
        }
    }
    intervalLengthFinal = (absMax - absMin) / intervalCount;


    for (experimentCounter = 0; experimentCounter <experimentCount ; experimentCounter++){
        evaluatedValue = evaluate(postArray, lineArr, intervalCount);
        if ((evaluatedValue < absMax) && (evaluatedValue > absMin)){
            i = (evaluatedValue - absMin) / intervalLengthFinal;
            finalProbabilityArray[i] += 1;
            confirmedExperiments += 1;
        }       
    }

    for (j = 0; j < intervalCount; j++){
        finalProbabilityArray[j] = finalProbabilityArray[j] / confirmedExperiments;
    }

    printf("%lf %lf ", absMin, absMax);
    for (j = 0; j < intervalCount; j++){
        printf("%lf ",  finalProbabilityArray[j]); 
    }

    return 0;
}

