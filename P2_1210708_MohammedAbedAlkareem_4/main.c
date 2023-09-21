#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

typedef struct charNode charStack;
typedef struct charNode PtrTocharNode;
struct charNode
{
    char Data;
    charStack* Next;
};

typedef struct doubleNode doubleStack;
typedef struct doubleNode PtrToDoubleNode;
struct doubleNode
{
    double Data;
    doubleStack* Next;
};
/////////////////////////////////char Stack
charStack* CreateCharStack();
int IsEmptyChar(charStack*);
char PopChar(charStack* );
void MakeEmptyCharStack(charStack*);
void PushChar(char, charStack*);
char TopChar(charStack*);
void DisposeCharStack(charStack*);
///////////////////////////////////Double Stack
doubleStack* CreateDoubleStack();
int IsEmptyDouble(doubleStack*);
double PopDouble(doubleStack* );
void MakeEmptyDoubleStack(doubleStack*);
void PushDouble(double, doubleStack*);
double TopDouble(doubleStack*);
void DisposeDoubleStack(doubleStack*);
/////////////////////////////////
char** readFile(int*,int*);
int numberCount(FILE*);
int MaxChar(FILE*);
//////////////////////////////////
void mainMenu();
void printEquations(char**,int* );
void DisposeEquations(char**,int*);
///////////////////////////////////
int IsDigit(char);
int OperatorPrecedence(char );
//////////////////////////////////Prefix conversion
void InfixConversion(char*, char*);
char matchingParanthesis(char );
char* ReverseEquation(char*);
///////////////////////////////////// validity
int IsValidEquation(char*);
int IsBalancedSymbols(char*);
int IsValidOperators(char*);
int IsValidParanthesis(char*);
int IsValidNumbers(char*);
///////////////////////////////////prefix evaluation
double PrefixEvaluation(char*);
double StringToDouble(char*);
/////////////////////////////////
void PrintToFile(char**, int*, int*, double*, int );

int main()
{
    printf("\nWelcome To Calculator ^_^\n\n");

    int num=0;
    int chars;

    char**Equations=NULL; // 2_D array to store the eqs in

    int* validEq=NULL, *invalidEq=NULL; // two arrays of integers to store valid and invalid eq indecies in
    int numOfValid = 0, numOfInvalid=0;

    char**PrefixEquations=NULL; // 2_D array to store the postfix eqs in

    double* Results =NULL; // array of doubles to store the results in it

    while (1) //always ask the user for the choice
    {
        int choice;

        // Display the main menu
        mainMenu();
        printf("What do you want to do: ");
        scanf("%d", &choice);
        printf("\n");

        switch (choice)
        {
        case 1:

            if (Equations != NULL){//if it was allocated before free it
                  DisposeEquations(Equations,&num);
                Equations=NULL;
            }

            if (PrefixEquations != NULL){//if it was allocated before free it
                DisposeEquations(PrefixEquations,&numOfValid);
                PrefixEquations = NULL;
            }


            if (validEq != NULL){//if it was allocated before free it
                free(validEq);
                validEq = NULL;
            }


            if (invalidEq != NULL){//if it was allocated before free it
                free(invalidEq);
                invalidEq = NULL;
            }

            if (Results != NULL){//if it was allocated before free it
                free(Results);
                 Results = NULL;
            }

            printf("Reading numbers......\n");
            Equations = readFile(&num,&chars);

            if (Equations != NULL)
                printf("The Equations have been read\n");

            //  to check if there is empty eq and delete it
            for (int i = 0; i < num ; i++)
            {
                if (strlen(Equations[i]) == 0)
                {
                    for (int j = i; j < num - 1; j++)
                    {
                        Equations[j] = Equations[j + 1];
                    }
                    num--; // Decrement the number of elements in the array
                    i--;
                }
            }
            printEquations(Equations,&num);
            break;

        case 2:
            // Check that input Equations have been read first
            if (Equations == NULL)
            {
                printf("\nYou should read the Equations first *_*\n");
                break;
            }

            if (validEq != NULL){//if it was allocated before free it
                free(validEq);
                validEq = NULL;
            }


            if (invalidEq != NULL){//if it was allocated before free it
                free(invalidEq);
                invalidEq = NULL;
            }


            numOfValid = 0, numOfInvalid=0;
            validEq = (int*)malloc(sizeof(int) * num); // Array to store the indices of the valid eqs
            invalidEq = (int*)malloc(sizeof(int) * num); // Array to store the indices of the invalid eqs

            int v = 0, iv = 0;
            for (int i = 0; i < num; i++)
            {
                if (IsValidEquation(Equations[i]) == 1) //if the eq was valid store its index
                {
                    validEq[v++] = i;
                    numOfValid++;
                }
                else //if the eq was invalid store its index
                {
                    invalidEq[iv++] = i;
                    numOfInvalid++;
                }
            }
            printf("Equation validity checked successfully!\n\n");

            iv=0;
            v=0;
            for(int i=0 ; i<num ; i++) // prints the results of the validity
            {
                if(i == invalidEq[iv]) //if the index of the eq is stored in the invalid array then the eq is invalid
                {
                    printf("Equation No.%d ==> Invalid \n",i+1);
                    iv++;
                }
                else if(i == validEq[v])//if the index of the eq is stored in the valid array then the eq is valid
                {
                    printf("Equation No.%d ==> Valid \n",i+1);
                    v++;
                }
            }

            break;

        case 3:

            if (Equations == NULL) // Check that input Equations have been read first
            {
                printf("\nYou should read the Equations first *_*\n");
                break;
            }

            if (validEq == NULL && invalidEq == NULL)// Check if equation validity has been checked
            {
                printf("You should check the validity of equations by choosing choice 2 \n");
                break;
            }

            if (numOfValid==0)
            {
                printf("There is no Valid Equations to be converted *_* \n\n");
                break;
            }

            if (PrefixEquations != NULL){//if it was allocated before free it
                DisposeEquations(PrefixEquations,&numOfValid);
                PrefixEquations = NULL;
            }

            PrefixEquations= (char**)malloc(sizeof(char*)*(numOfValid)); //array of strings to store the prefix expression
            if (PrefixEquations == NULL)
            {
                printf("Memory allocation failed.\n");
                exit(0);
            }

            for(int i=0; i<numOfValid; i++) //the conversion is only for valid eq
            {

                PrefixEquations[i]=(char*)malloc(sizeof(char)*(chars));

                if (PrefixEquations[i] == NULL)
                {
                    printf("Memory allocation failed.\n");
                    exit(0);
                }

                strcpy(PrefixEquations[i],"");
                InfixConversion(Equations[validEq[i]], PrefixEquations[i]);
            }

            printf("Equations have been successfully converted!\n\n");

            for(int i = 0 ; i<numOfValid ; i++)//for printing the prefix eq
                printf("Equation No.%d:%s \n",validEq[i]+1,PrefixEquations[i] );

            break;

        case 4:

            if (Equations == NULL)// Check that input Equations have been read first
            {
                printf("\nYou should read the Equations first *_*\n");
                break;
            }

            if (PrefixEquations == NULL) //checks if the eqs have been converted
            {
                printf("\nYou should convert the Equations to prefix first by choosing choice 3 *_*\n");
                break;
            }


            if (Results != NULL){//if it was allocated before free it
                free(Results);
                 invalidEq = NULL;
            }

            Results = (double*)malloc(sizeof(double) * numOfValid); //array of doubles to store the results in it

            if (Results == NULL)
            {
                printf("Memory allocation failed.\n");
                exit(0);
            }

            for(int i=0; i<numOfValid; i++) //evaluate each prefix eq
                Results[i]=PrefixEvaluation(PrefixEquations[i]);

            printf("Equations have been successfully evaluated!\n\n");

            for(int i = 0 ; i<numOfValid ; i++) //print the result of the prefix eq with 3 digits after the floating point
            {
                if(Results[i]!=-999)
                    printf("Equation No.%d: the result = %.3lf \n",validEq[i]+1, Results[i]);

                else
                    printf("Equation No.%d: the result = MATH ERROR -dividing by 0-  \n",validEq[i]+1);
            }


            break;

        case 5:

            if (Equations == NULL)// Check that input Equations have been read first
            {
                printf("\nYou should read the Equations first *_*\n");
                break;
            }

            if (validEq == NULL && invalidEq == NULL)// Check if equation validity has been checked
            {
                printf("You should check the validity of equations by choosing choice 2 \n");
                break;
            }

            if (numOfInvalid == 0)// Check if there are any invalid equations
            {
                printf("All equations are valid ^_^ \n");
                break;
            }

            printf("Invalid Equations:\n");  // Print the invalid equations
            for (int i = 0; i < numOfInvalid; i++)
            {
                int index = invalidEq[i];
                printf("Equation %d: %s\n", index+1, Equations[index]);
            }
            break;

        case 6:

            if (Equations == NULL)// Check that input Equations have been read first
            {
                printf("\nYou should read the Equations first *_*\n");
                break;
            }

            if (PrefixEquations == NULL) //checks if the eqs have been converted
            {
                printf("\nYou should convert the Equations to prefix first by choosing choice 3 *_*\n");
                break;
            }

            if (Results == NULL) //checks if the eqs have been evaluated
            {
                printf("\nYou should evaluate the prefix Equations first by choosing choice 4 *_*\n");
                break;
            }
            printf("Printing Outputs.......\n");
            PrintToFile(PrefixEquations, invalidEq,validEq,  Results, num );
            printf("Outputs have been successfully printed ^_^\n");

            break;

        case 7:

            // Free the memory allocated for the input numbers and the results queue
            if (Equations != NULL)
            {
                printf("....Disposing Equations\n");
                DisposeEquations(Equations,&num);

                //free what have been allocated
                if(PrefixEquations != NULL)
                    DisposeEquations(PrefixEquations,&numOfValid);

                if(validEq != NULL)
                    free(validEq);

                if(invalidEq != NULL)
                    free(invalidEq);

                if(Results != NULL)
                    free(Results);
            }

            printf("Thanks for using Calculator ^_^\n");
            exit(0);

        default:
            printf("Enter a choice between 1 and 7\n");
        }
    }
    return 0;

}

/*
*returns 1 if the stack is empty otherwise return 0
*/
int IsEmptyChar(charStack*S)
{
    return S->Next==NULL;
}

/*
*allocate stack
*/
charStack* CreateCharStack()
{
    charStack* S;

    S=(charStack*)malloc(sizeof(charStack));
    if(S==NULL)
        printf("Out of Memory *_*");

    S->Next=NULL;

    return S;
}
 /*
 *returns the top element in the stack (and delete it)
 */
char PopChar(charStack*S)
{
    PtrTocharNode* Top;
    Top=S->Next;
    char x='!';

    if(Top!=NULL)
    {
        x= Top->Data;
        S->Next=Top->Next;
        free(Top);
    }
    return x;
}

/*
*delete the elements in the stack
*/
void MakeEmptyCharStack(charStack*S)
{
    if(IsEmptyChar(S))
        return;

    else
    {
        do
        {
            PopChar(S);
        }
        while(!IsEmptyChar(S));
    }
}

/*
*stores element in the top of the stack
*/
void PushChar(char x, charStack*S)
{
    PtrTocharNode* temp;

    temp = (PtrTocharNode*)malloc(sizeof(PtrTocharNode));
    if(temp==NULL)
        printf("out of memory *_*");
    else
    {
        temp->Data=x;
        temp->Next=S->Next;
        S->Next=temp;
    }
}

/*
*returns the value that is stored in the top of the stack
*/
char TopChar(charStack*S)
{
    if(!IsEmptyChar(S))
        return S->Next->Data;

    printf("Empty Stack!!");
    return '!';
}

/*
*dispose the stack
*/
void DisposeCharStack(charStack*S)
{
    MakeEmptyCharStack(S);
    free(S);
}

/*
*a function that reads the eq in the file and returns 2_D array the stores the eq
*update the value of the number of eq and the max num of char in eq
*/
char** readFile(int* num,int*maxChar)
{
    char filename[100];
    printf("Enter file name -including file type- : ");
    scanf("%s", filename);
    FILE *fin = fopen(filename, "r");

    while (fin == NULL)
    {
        printf("Failed to open file: %s\n", filename);

        printf("\nEnter file name -including file type- correctly: ");
        scanf("%s", filename);
        fin = fopen(filename, "r");
    }

    *num = numberCount(fin);
    fseek(fin, 0, SEEK_SET);// Reset the file pointer to the beginning of the file

    *maxChar = MaxChar(fin);
    fseek(fin, 0, SEEK_SET);// Reset the file pointer to the beginning of the file

    char** charaterOfEq= (char**)malloc(sizeof(char*)*(*num));//allocate the 2_D array
    if (charaterOfEq == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(0);
    }

    for(int i=0; i<*num; i++) //allocate arrays of chars
    {
        charaterOfEq[i]=(char*)malloc(sizeof(char)*(*maxChar));

        if (charaterOfEq[i] == NULL)
        {
            printf("Memory allocation failed.\n");
            exit(0);
        }

        strcpy(charaterOfEq[i],"");
    }

    char c;

    int i=0,j=0;

    while((c=fgetc(fin))!=EOF)//read char by char and store in the arrays
    {
        if(c=='\n')//id \n is the char then new eq starts
        {
            charaterOfEq[i][j] = '\0';
            i++;
            j=0;
        }
        else
        {
            charaterOfEq[i][j]=c;
            j++;
        }
    }

    fclose(fin);

    return charaterOfEq;
}

/*
*returns the number of eq in file fin
*/
int numberCount(FILE* fin)
{
    char ch;
    int lines = 1;

    while ((ch = fgetc(fin)) != EOF)
    {
        // If we find a newline character, increment the line counter
        if (ch == '\n')
        {
            lines++;
        }
    }

    return lines;
}

/*
*returns the max number of char in eq
*/
int MaxChar(FILE*fin)
{
    char c;
    int chars=0, maxChars=0;

    while ((c = fgetc(fin)) != EOF)
    {
        if(c!='\n')
            chars ++;

        else
        {
            if(chars>maxChars)
                maxChars=chars;

            chars=0;
        }
    }
    if(chars>maxChars)
        maxChars=chars;

    return maxChars;
}

/*
*prints the 2_D array in appropriate way
*/
void printEquations(char**Eqs,int*num)
{
    int counter=1;
    for(int i=0 ; i<*num; i++)
    {
        printf("Equation %d: %s\n", counter++, Eqs[i]);
    }
}

/*
*dellocate the 2_D array
*/
void DisposeEquations(char**eq,int*num)
{
    for(int i=0 ; i<(*num) ; i++)
        free(eq[i]);

    free(eq);
}

/*
*Prints the Main Menu
*/
void mainMenu()
{
    printf("\nChoose from the below operations:\n\n");
    printf("1. Read the Equations from File\n");
    printf("2. Check the validity of equations\n");
    printf("3. Convert valid equations from infix to prefix\n");
    printf("4. Evaluate the prefix\n");
    printf("5. Print invalid Equations\n");
    printf("6. print the results to	an output file\n");
    printf("7. Exit\n");
}

/*
*returns if the char is digit or not
*/
int IsDigit(char c)
{
    int ascii = c-48;
    if(ascii>=0 && ascii<=9)
        return 1;
    else
        return 0;
}

/*
*returns the Operator Precedence
*if it was not operator it returns 0
*/
int OperatorPrecedence(char c)
{
    if(c=='^')
        return 3;
    else if (c=='*'||c=='/')
        return 2;
    else if (c=='+'||c=='-')
        return 1;
    else
        return 0;
}

/*
*checks if the eq has balanced symbols
*/
int IsBalancedSymbols(char*eq)
{
    charStack*openStack;
    openStack=CreateCharStack();

    char P;
    for (int i = 0; i < strlen(eq); i++)
    {
        P =eq[i];
        switch(P)
        {
        case '[':
            PushChar('[',openStack);
            break;

        case '{':
            PushChar('{',openStack);
            break;

        case '(':
            PushChar('(',openStack);
            break;

        //in  case of closing we have to check
        case ']':
            if(PopChar(openStack)!='[')
                return 0;
            break;

        case '}':
            if(PopChar(openStack)!='{')
                return 0;
            break;

        case ')':
            if(PopChar(openStack)!='(')
                return 0;
            break;
        }
    }
    if (IsEmptyChar(openStack))
        return 1;
    else
        return 0;
}

/*
*checks the validity based on the operations
*/
int IsValidOperators(char* eq)
{
    if (OperatorPrecedence(eq[0]) > 0)
        return 0;  // If the precedence of the first character is operator, return 0

    char P;
    int IsOperator = 0;

    for (int i = 0; i < strlen(eq); i++)
    {
        P = eq[i];

        if ((P == '+' || P == '-' || P == '*' || P == '/') && IsOperator == 1)
            return 0;  // If there are consecutive operators, return 0 (invalid operators)
        else if (P == '+' || P == '-' || P == '*' || P == '/')
            IsOperator = 1;  // Set IsOperator to 1 if the current character is an operator
        else if (P != ' ')
            IsOperator = 0;  // Reset IsOperator to 0 if the current character is not an operator or a space

        //to check if there is any alphabet char
        if(isalpha(P))
            return 0;
    }

    if (IsOperator == 1) // If the last character is an operator, return 0 (invalid operators)
        return 0;
    else
        return 1;  // All operators are valid, so return 1
}

/*
*checks the validity based on the chars before and after the parenthesis
*/
int IsValidParanthesis(char*eq)
{
    for (int i = 0; i < strlen(eq); i++)
    {
        char P = eq[i];

        if (P=='(' || P=='['||P=='{')
        {

            // Check if the character before the opening bracket is a digit or closing parentheses
            if (i > 1 && (IsDigit(eq[i - 2]) || (P==')' || P==']'||P=='}')  ) )
                return 0;

            // Check if the character after the opening bracket is an operator or closing parentheses
            if (i < strlen(eq) - 2 && (OperatorPrecedence(eq[i + 2]) || (P==')' || P==']'||P=='}')))
                return 0;
        }

        if (P==')' || P==']'||P=='}')
        {

            // Check if the character before the closing bracket is an operator or opening parentheses
            if (i > 1 && (OperatorPrecedence(eq[i - 2])  || (P=='(' || P=='['||P=='{')  )   )
                return 0;


            // Check if the character after the closing bracket is a digit or opening parentheses
            if (i < strlen(eq) - 2 && (IsDigit(eq[i + 2]) || (P=='(' || P=='['||P=='{')))
                return 0;
        }
    }
    return 1;
}

/*
*checks for two consecutive numbers
*/
int IsValidNumbers(char*eq)
{
    char* eqCopy = strdup(eq);

    char* token = strtok(eqCopy, " ");

    int IsNum=0;

    while (token != NULL)
    {
        if(IsDigit(token[0]))
        {
            if(IsNum==0)
                IsNum=1;

            else if(IsNum==1)
                return 0;
        }
        else
            IsNum=0;

        token=strtok(NULL, " ");
    }
    free(eqCopy);
    return 1;

}

/*
*if the eq satisfies all the conditions to be valid the returns 1 otherwise returns 0
*/
int IsValidEquation(char*eq)
{
    if(IsBalancedSymbols(eq)&& IsValidOperators(eq)&&IsValidParanthesis(eq)&& IsValidNumbers(eq))
        return 1;
    else
        return 0;
}

/*
*a function that converts infix to prefix expression
*/
void InfixConversion(char* eq, char* preEq)
{
    char* revEq = ReverseEquation(eq); //reverse the infix and fix the parenthesis

    charStack* Operators = NULL;
    Operators = CreateCharStack();
    int preEqIndex = 0;

    for (int i = 0; i < strlen(revEq); i++)
    {
        if (revEq[i] == ' ' || IsDigit(revEq[i])|| revEq[i] == '.')
        {
            preEq[preEqIndex++] = revEq[i];  // Copy digits and spaces directly to the prefix expression
        }
        else if (revEq[i] == '(' || revEq[i] == '[' || revEq[i] == '{')
        {
            PushChar(revEq[i], Operators);  // Push opening parenthesis onto the stack
            i++; // Skip the next space
        }
        else if (revEq[i] == ')' || revEq[i] == '}' || revEq[i] == ']')
        {
            // Pop operators from the stack and add them to the prefix expression until an opening parenthesis is encountered
            while (!IsEmptyChar(Operators) && (TopChar(Operators) != matchingParanthesis(revEq[i])))
            {
                preEq[preEqIndex++] = PopChar(Operators);
                preEq[preEqIndex++] = ' ';
            }


            PopChar(Operators); // Pop the matching opening parenthesis from the stack
            i++; // Skip the next space
        }
        else
        {
            // Pop operators from the stack and add them to the prefix expression until an operator with just lower precedence is reached or the stack is empty
            while (!IsEmptyChar(Operators) && OperatorPrecedence(TopChar(Operators)) > OperatorPrecedence(revEq[i]))
            {
                preEq[preEqIndex++] = PopChar(Operators);
                preEq[preEqIndex++] = ' ';
            }

            // Push the current operator onto the stack
            PushChar(revEq[i], Operators);
            i++; // Skip the next space
        }
    }

    // Pop any remaining operators from the stack and add them to the prefix expression
    while (!IsEmptyChar(Operators))
    {
        preEq[preEqIndex++] = PopChar(Operators);
        preEq[preEqIndex++] = ' ';
    }

    // Add null-terminator to the prefix expression
    preEq[preEqIndex] = '\0';

    strrev(preEq); // reverse the expression to reach the correct expression

    DisposeCharStack(Operators);
    free(revEq);

}
/*
*returns the opening parenthesis of the closing ones
*/
char matchingParanthesis(char c)
{
    switch (c)
    {
    case ')':
        return '(';
        break;

    case '}':
        return '{';
        break;

    case ']':
        return '[';
        break;
    }

    return 0;
}

/*
*returns a string that is reversed and with fixed parenthesis
*/
char* ReverseEquation(char*eq)
{
    char* revEq = (char*)malloc(sizeof(char)*strlen(eq));
    strcpy(revEq,eq);
    revEq=strrev(revEq); //reverse the string

    for(int i =0 ; i<strlen(revEq); i++) //correcting the parenthesis
    {
        switch(revEq[i])
        {
        case '(':
            revEq[i]=')';
            break;

        case ')':
            revEq[i]='(';
            break;

        case '{':
            revEq[i]='{';
            break;

        case '}':
            revEq[i]='{';
            break;

        case '[':
            revEq[i]=']';
            break;

        case ']':
            revEq[i]='[';
            break;
        }
    }
    return revEq;
}

doubleStack* CreateDoubleStack()
{
    doubleStack* S;
    S=(doubleStack*)malloc(sizeof(doubleStack));
    if(S==NULL)
        printf("Out of Memory *_*");

    S->Next=NULL;

    return S;
}

double PopDouble(doubleStack*S)
{
    PtrToDoubleNode* Top;
    Top=S->Next;
    double x;

    if(Top!=NULL)
    {
        x= Top->Data;
        S->Next=Top->Next;
        free(Top);
    }
    return x;
}

void MakeEmptyDoubleStack(doubleStack*S)
{
    if(IsEmptyDouble(S))
        return;
    else
    {
        do
        {
            PopDouble(S);
        }
        while(!IsEmptyDouble(S));
    }
}

void PushDouble(double x, doubleStack*S)
{
    PtrToDoubleNode* temp;

    temp = (PtrToDoubleNode*)malloc(sizeof(PtrToDoubleNode));
    if(temp==NULL)
        printf("out of memory *_*");
    else
    {
        temp->Data=x;
        temp->Next=S->Next;
        S->Next=temp;
    }
}

double TopDouble(PtrToDoubleNode*S)
{

    if(!IsEmptyDouble(S))
        return S->Next->Data;

    printf("Empty Stack!!");
    return 0;
}

int IsEmptyDouble (doubleStack * s)
{
    return s->Next==NULL;
}

void DisposeDoubleStack(doubleStack*S)
{
    MakeEmptyDoubleStack(S);
    free(S);
}

/*
*evaluate the value of the prefix expression
*/
double PrefixEvaluation(char* eq)
{
    // Create a stack to store numbers
    doubleStack* nums = CreateDoubleStack();
    double result = 0;

// Create a copy of the equation string
    char* eqCopy = strdup(eq);
    if (eqCopy == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    // Reverse the equation string
    strrev(eqCopy);

    // Token the equation
    char* token = strtok(eqCopy, " ");
    while (token != NULL)
    {
        // Reverse the token to get the original number
        strrev(token);

        if (IsDigit(token[0])) // Check if the token is a number
            PushDouble(StringToDouble(token), nums); // Convert the token to double and push it to the stack

        else // The token is an operator
        {
            double num1, num2;
            num1 = PopDouble(nums); // Pop the top number from the stack
            num2 = PopDouble(nums); // Pop the second top number from the stack

            switch (token[0])// choosing the correct operation to be performed and push the result back to the stack
            {
            case '+':
                result = num1 + num2;
                PushDouble(result, nums);
                break;

            case '-':
                result = num1 - num2;
                PushDouble(result, nums);
                break;

            case '*':
                result = num1 * num2;
                PushDouble(result, nums);
                break;

            case '^':
                result = pow(num1,num2 );
                PushDouble(result, nums);
                break;

            case '/':
                if (num2 == 0)
                    return -999; // Division by zero error
                result = num1 / num2;
                PushDouble(result, nums);
                break;
            }
        }
        token = strtok(NULL, " "); // Get the next token
    }

    result = PopDouble(nums); // Pop the final result from the stack

    if (!IsEmptyDouble(nums))
    {
        printf("Error: Invalid prefix expression\n");
        exit(0);
    }
    free(eqCopy);
    return result;
}
/*
*returns the double value for the token string
*/
double StringToDouble(char*num)
{
    double number = 0 ;

    int intDigit = 0, floatDigit = 1;

    int IsFlotingPoint = 0 ;
    for(int i = 0; i <strlen(num); i++) // Iterate through each character in the string
    {
        if(num[i]=='.')   // Check if the character is a decimal point
        {
            IsFlotingPoint = 1 ;
            continue;
        }
        if(IsDigit(num[i]) && IsFlotingPoint==0)//If the character is a digit and a floating point flag is 0 Calculate the integer part of the number
            number= (number* pow(10,intDigit++))+(num[i]-48);

        else if (IsDigit(num[i]) && IsFlotingPoint==1) // If the character is a digit and a floating point flag is 1 Calculate the fractional part of the number
            number+= (num[i]-48) / pow(10,floatDigit++);

        // Reset the floating point flag
        IsFlotingPoint = 0 ;
    }
    return number;
}
/*
*prints the output of the execution to a file thats name is from the user in appropriate way
*/
void PrintToFile(char** PrefixEquations, int*invalidEq, int*validEq, double*Results, int num )
{
    char filename[100];
    printf("Enter file name -including file type- : ");
    scanf("%s", filename);
    FILE *fout = fopen(filename, "w");

    while (fout == NULL)
    {
        printf("Failed to open file: %s\n", filename);

        printf("\nEnter file name -including file type- correctly: ");
        scanf("%s", filename);
        fout = fopen(filename, "r");
    }

    int iv=0, v=0,counter = 1;
    for(int i=0 ; i<num ; i++)
    {
        if(i == invalidEq[iv])
        {
            fprintf(fout,"Equation No.%d ==> Invalid \n",counter++);
            iv++;
        }
        else if(i == validEq[v])
        {
            if(Results[v]!=-999)//if the result is -999 then it is math error
                fprintf(fout,"Equation No.%d ==> Valid , the Prefix Expression %s , the result = %.3lf \n",counter++, PrefixEquations[v], Results[v]);
            else
                fprintf(fout,"Equation No.%d ==> Valid , the Prefix Expression %s , the result = MATH ERROR -dividing by 0- \n",counter++, PrefixEquations[v]);
            v++;
        }
    }
    fclose(fout);
}
