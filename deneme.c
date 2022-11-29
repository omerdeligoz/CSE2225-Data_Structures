//Omer DELIGOZ 150120035

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct node {   //define a node structure
    int data;
    struct node *next;
};
typedef struct node node;

void printList(node *head);

int createLists(FILE *inputFilePtr, node **multiplicandPtr, node **multiplierPtr, node **resultPtr, int *base);

node *addNode(int data, node *head);

node *convertToDecimal(node **myPtr, node **resultPtr, int base);

void reverseList(struct node **head);

void multiply(node *multiplicandPtr, node *multiplierPtr, node *resultPtr, int base);

void printToFile(node *multiplicandPtr, node *multiplierPtr, node *resultPtr); // (no longer used)

//Create multiplicand and multiplier linked lists based on the given input file and create an empty result list
//If there is any problem with the input show an error message and return
int createLists(FILE *inputFilePtr, node **multiplicandPtr, node **multiplierPtr, node **resultPtr, int *base) {
    char inputChar;
    int count = 0;   //This is for result linked list. When multiplying two numbers, the result consists of
    //as many digits as the sum of the digits of the two numbers +1

    // read input file and create a linked list for multiplicand number
    while (((inputChar = fgetc(inputFilePtr)) != EOF) && (inputChar != '\n')) {
        if (inputChar < '0' || inputChar > '9') {
            printf("There is an invalid character in multiplicand value");
            return 0;
        }
        *multiplicandPtr = addNode(atoi(&inputChar),
                                   *multiplicandPtr); //Add a node to the linked list using the character read from the file
        count++;
    }

    // read input file and create a linked list for multiplier number
    while (((inputChar = fgetc(inputFilePtr)) != EOF) && (inputChar != '\n')) {
        if (inputChar < '0' || inputChar > '9') {
            printf("There is an invalid character in multiplier value");
            return 0;
        }
        *multiplierPtr = addNode(atoi(&inputChar),
                                 *multiplierPtr); //Add a node to the linked list using the character read from the file
        count++;
    }

    char baseArr[2];
    fgets(baseArr, 3, inputFilePtr);    //read input file and assign the line to baseArr
    if (atoi(baseArr) > 10 || atoi(baseArr) < 2) {     //check if the base input valid or not
        printf("The base must be 2 <= k <= 10");
        return 0;
    } else {
        *base = atoi(baseArr);      //set as base if input is valid
    }

    //close the input file
    fclose(inputFilePtr);

    //Check if there is a number greater than the base value in the input, if so, give an error message and return
    node *tempMultiplicandPtr = *multiplicandPtr;
    node *tempMultiplierPtr = *multiplierPtr;

    while (tempMultiplicandPtr->next != NULL) {
        if (tempMultiplicandPtr->data >= *base) {
            printf("The multiplicand contains a number greater than the base");
            return 0;
        }
        tempMultiplicandPtr = tempMultiplicandPtr->next;
    }
    while (tempMultiplierPtr->next != NULL) {
        if (tempMultiplierPtr->data >= *base) {
            printf("The multiplier contains a number greater than the base");
            return 0;
        }
        tempMultiplierPtr = tempMultiplierPtr->next;
    }
    //Create result list as all zeros
    for (int i = 0; i < count; ++i) {
        *resultPtr = addNode(0, *resultPtr);
    }
    return 1;
}

node *addNode(int data, node *head) {  //add a new node to the head of the linked list
    node *newNode;
    newNode = (node *) malloc(sizeof(node));
    newNode->data = data;
    newNode->next = head;
    head = newNode;
    return head;
}

//convert linkedlist in given base to decimal.
//Overflow may occur as it is necessary to keep the sum value in an int variable !!!
node *convertToDecimal(node **myPtr, node **resultPtr, int base) {
    int sum = 0;
    node *tempPtr = *myPtr;
    node *decimal = NULL;

    //calculate the decimal value of the given number
    for (int i = 0; (tempPtr != NULL); ++i) {
        sum += (tempPtr->data) * (int) pow(base, i);
        tempPtr = tempPtr->next;
    }

    //convert found decimal number to linkedlist
    while (sum > 0) {
        decimal = addNode(sum % 10, decimal);
        *resultPtr = addNode(0, *resultPtr);
        sum /= 10;
    }
    return decimal;
}

//Reverse the given linked list
void reverseList(node **head) {
    struct node *previous = NULL;
    struct node *current = *head;
    struct node *next = NULL;

    while (current != NULL) {
        next = current->next;
        current->next = previous;
        previous = current;
        current = next;
    }
    *head = previous;
}

//Multiply the two given lists with respect to the base and write the result in the result linkedlist
void multiply(node *multiplicandPtr, node *multiplierPtr, node *resultPtr, int base) {
    int carry = 0;
    int value = 0;          //Multiplicand * multiplier + carry
    int oldResult = 0;
    int multiplicand, multiplier;

    //define some temporary pointers
    node *tempResultPtr = resultPtr;
    node *currentResultPtr = tempResultPtr;
    node *currentMultiplicandPtr = multiplicandPtr;
    node *currentMultiplierPtr = multiplierPtr;

    //this loop is for multiplying two linked lists
    while (currentMultiplierPtr != NULL) {
        while (currentMultiplicandPtr != NULL) {
            multiplicand = currentMultiplicandPtr->data;
            multiplier = currentMultiplierPtr->data;
            oldResult = currentResultPtr->data;

            value = multiplicand * multiplier + carry;
            currentResultPtr->data = (currentResultPtr->data + value) % base;
            carry = (oldResult + value) / base;

            if (currentMultiplicandPtr->next == NULL) {
                oldResult = currentResultPtr->next->data;
                currentResultPtr->next->data = (currentResultPtr->next->data + carry) % base;
                carry = (oldResult + carry) / base;
            }
            currentMultiplicandPtr = currentMultiplicandPtr->next;
            currentResultPtr = currentResultPtr->next;
        }
        currentMultiplierPtr = currentMultiplierPtr->next;  //shift the current Multiplier by one
        tempResultPtr = tempResultPtr->next;                //shift the result pointer by one
        currentResultPtr = tempResultPtr;
        currentMultiplicandPtr = multiplicandPtr;
    }
}

void printList(node *head) {
    node *current_node = head;
    if (current_node->data == 0) {    //if the first element is zero, skip it
        current_node = current_node->next;
    }
    //print entire linkedlist one by one
    while (current_node != NULL) {
        printf("%d", current_node->data);
        current_node = current_node->next;
    }
}

//print given linked lists to an output file (no longer used)
void printToFile(node *multiplicandPtr, node *multiplierPtr, node *resultPtr) {
    FILE *outputFilePtr;

    //Reverse all lists to print
    reverseList(&multiplicandPtr);
    reverseList(&multiplierPtr);
    reverseList(&resultPtr);

    //Open a file for output. Show eror message if error occurs
    if ((outputFilePtr = fopen("output.txt", "a")) != NULL) {

        //Print multiplicand
        while (multiplicandPtr != NULL) {
            fprintf(outputFilePtr, "%d", multiplicandPtr->data);
            multiplicandPtr = multiplicandPtr->next;
        }
        putc('\n', outputFilePtr);  //print line break

        //Print multiplier
        while (multiplierPtr != NULL) {
            fprintf(outputFilePtr, "%d", multiplierPtr->data);
            multiplierPtr = multiplierPtr->next;
        }
        putc('\n', outputFilePtr);  //print line break

        //print result
        if (resultPtr->data == 0) {    //if the first element is zero, skip it
            resultPtr = resultPtr->next;
        }
        while (resultPtr != NULL) {
            fprintf(outputFilePtr, "%d", resultPtr->data);
            resultPtr = resultPtr->next;
        }
        putc('\n', outputFilePtr);
    } else {
        printf("Output file could not create");
    }

    //Close the output file
    fclose(outputFilePtr);
}


int main(int argc, char *argv[]) {
    if (argc == 1) {    //print an error message if no arguments
        printf("Error message!");
        return -1;
    }
    if (argc >= 2) {
        FILE *inputFilePtr = fopen(argv[1], "r"); //open the given file
        //define necessary variables
        node *multiplicandPtr = NULL;
        node *multiplierPtr = NULL;
        node *resultPtr = NULL;
        node *decimalMultiplicandPtr = NULL;
        node *decimalMultiplierPtr = NULL;
        node *decimalResultPtr = NULL;
        int base;
        int *basePtr = &base;

        //call method to create lists, if there is an error , return -1 and exit the program
        if (createLists(inputFilePtr, &multiplicandPtr, &multiplierPtr, &resultPtr, basePtr) == 0) {
            return -1;
        }
        //convert given numbers to decimal
        decimalMultiplicandPtr = convertToDecimal(&multiplicandPtr, &decimalResultPtr, base);
        decimalMultiplierPtr = convertToDecimal(&multiplierPtr, &decimalResultPtr, base);

        //reverse decimal numbers to multiply
        reverseList(&decimalMultiplicandPtr);
        reverseList(&decimalMultiplierPtr);

        //call function to multiply linked lists
        multiply(multiplicandPtr, multiplierPtr, resultPtr, *basePtr);
        multiply(decimalMultiplicandPtr, decimalMultiplierPtr, decimalResultPtr, 10);

        //Reverse all lists to print
        reverseList(&multiplicandPtr);
        reverseList(&multiplierPtr);
        reverseList(&resultPtr);
        reverseList(&decimalMultiplicandPtr);
        reverseList(&decimalMultiplierPtr);
        reverseList(&decimalResultPtr);


        //Print all values to console
        printList(multiplicandPtr);
        printf("\n");
        printList(multiplierPtr);
        printf("\n");
        printList(resultPtr);
        printf("\n");
        printList(decimalMultiplicandPtr);
        printf("\n");
        printList(decimalMultiplierPtr);
        printf("\n");
        printList(decimalResultPtr);
    }
    return 0;
}