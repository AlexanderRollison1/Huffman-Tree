#include <stdio.h>  //gets
#include <stdlib.h> //atoi, malloc
#include <string.h> //strcpy

struct TreeNode
{
	char character;
	int frequency;
	struct TreeNode *left;
	struct TreeNode *right;
};

struct TreeQueue 
{
	int start;
	int end;
	int capacity;
	struct TreeNode **data;
};


//Decoding: Creating Tree and decode print
void WriteDecode(struct TreeNode *root, char str[]);
struct TreeNode* decode(FILE * fp);

//File and str getter functions
void strFromFile(char *str,FILE *FH);
FILE* OpenFile(char filename[], char mode[]);
int strCount(char str[], char ch);

//Encoding: Making Nodes and Building Tree
struct TreeNode* makeNode(char ch, int num);
struct TreeQueue * makeQueue(int capacity);
struct TreeQueue* buildTree(char character[], int frequency[], int size);
struct TreeNode* deQueue(struct TreeQueue* queue);
void enQueue(struct TreeQueue* queue, struct TreeNode* item);
struct TreeNode* Min(struct TreeQueue* Queue1,struct TreeQueue* Queue2);
struct TreeNode* front(struct TreeQueue* queue);
//Displays Trees:
//Write to screen 
void displayTree(struct TreeNode* root,int array[],int top); 
//Print tree nodes and print the encoded string
void PrintETree(struct TreeNode*root, FILE* fp);
void writeEncodeTree(struct TreeNode* root,int array[],int top, char ch,FILE* filename);

//Destroyer
void destroyList(struct TreeNode* root);
void destroyQueue(struct TreeQueue* root);

int main()
{
	
	//String sorting and finding frequency variables.
	char str[1000] = {};
	int length;
	char check[100]= {};
	int checkcount = 0;
	char ch;
	int chiter = 0;
	int count = 0;
	char character[100]= {};
	int frequency[100]= {};
	
	//valgrind?
	char zero = '0';
    char one = '1';
    char er1 = '\0';
	char er2 = '\n';
	
	
	//Files
	char input[50]= {};
	char output[50]= {};
	FILE * inputFILE = NULL;
	FILE * outputFILE = NULL;
	char mode[2] = "r+";
	
	//User choice
	int choice;
	
	printf("\n\nEncode(1) or Decode(2)? ");
	scanf("%d",&choice);
	//START OF ENCODING
	if(choice == 1)
	{
		printf("Enter the name of the DESTINATION file (encoded file): ");
		scanf("%s", output);
		printf("Enter the name of the SOURCE file (to be encoded): ");
		scanf("%s",input);
		
		inputFILE = OpenFile(input,mode);
		outputFILE = OpenFile(output,mode);
		strFromFile(str,inputFILE);
		
		ch = str[chiter];
		while ((ch != er1) && (ch != er2))
		{
			if((!(strchr(check,ch) != NULL)))
			{
				check[checkcount++] = ch;
				count = strCount(str, ch);
				printf("\n(%c,%d)", ch, count);
				character[checkcount - 1] = check[checkcount - 1];
				frequency[checkcount -1] = count;
			}
			chiter = chiter+1;
			if(chiter == length-1)
			{
				ch = er1;
			}
			else if(!(chiter == length))
			{
				ch = str[chiter];
			}
			else
			{
				ch = er1;
			}
		}
		
		
		int k, j, index; 
		int temp;
		char temp1;
		int size = strlen(check);
		for (k = 0; k < size - 1; k++) 
		{ 
			index = k; 
			for (j = k + 1; j < size; j++) 
			{
				if (frequency[j] < frequency[index]) 
				{
					index = j; 
				}
			}
			temp = frequency[index];
			frequency[index] = frequency[k];
			frequency[k] = temp;
			temp1 = character[index];
			character[index] = character[k];
			character[k] = temp1;
		} 
		struct TreeQueue* root1 = buildTree(character,frequency,size);
		struct TreeNode* root = deQueue(root1);
        //destroyQueue(root1);
		int array[100]= {};
		int top = 0;
		displayTree(root,array,top);
		int s;
		char ch1;
		char ch2;
		PrintETree(root,outputFILE);
		
		for(s = 0; s < strlen(str);s++)
		{
			ch1 = str[s];
			writeEncodeTree(root,array,top, ch1, outputFILE);
		}
		destroyList(root);
		fclose(inputFILE);
		fclose(outputFILE);
	
	}
	//END OF ENCODING
	
	//START OF DECODING
	else if(choice == 2)
	{
		printf("Enter the name of the DESTINATION file (encoded file): ");
		scanf("%s",input);
		
		inputFILE = OpenFile(input,mode);
		
		
		struct TreeNode* decodedTree = decode(inputFILE);
		
		fgets(str, sizeof(str)-1,inputFILE);
		if(str[strlen(str)-1] == er2)
		{
			str[strlen(str)-1] = er1;
		}
		WriteDecode(decodedTree, str);
		destroyList(decodedTree);
		fclose(inputFILE);
	}
	//END OF DECODING
	else
	{
		return 0;
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return 0;
}
void destroyQueue(struct TreeQueue* root)
{
    if(root != NULL)
    {
        destroyList(root->data[0]);
        free(root->data);
        free(root);
    }
}
void destroyList(struct TreeNode* root)
{
	if(root != NULL)
	{
		destroyList(root->left);
		destroyList(root->right);
		free(root);
	}
}

void WriteDecode(struct TreeNode *root, char str[])
{
	
	char ch;
	int i;
    char er1 = '0';
    int one = 1;
	struct TreeNode* temp = root;
	for(i = 0; i < strlen(str);i++)
	{
		if(str[i] == er1)
		{
			temp = temp->left;
		}
		else
		{
			temp = temp->right;
		}

		if(temp->frequency == one)
		{
			char printchar = temp->character; 
			printf("%c",printchar);
			temp = root;
		}
	}
	
}

struct TreeNode* decode(FILE * fp)
{
	char ch;
	ch = fgetc(fp);
	struct TreeNode* root = malloc(sizeof(struct TreeNode));
	root->character = ch;
	root->right = NULL;
	root->left = NULL;
	root->frequency = 0;
	char zero = '0';
    char one = '1';
	if(ch == zero)
	{
		root->left = decode(fp);
		root->right = decode(fp);
	}
	if(ch == one)
	{
		ch = fgetc(fp);
		struct TreeNode* newNode = malloc(sizeof(struct TreeNode));
		newNode->left = NULL;
		newNode->right = NULL;
		newNode->character = ch;
		newNode->frequency = 1;
		root = newNode;
	}
	return root;
}

void strFromFile(char *str,FILE *FH)
{
	char index[1000]= {};
	char er1 = '\0';
    char er2 = '\n';

	fgets(index, sizeof(index)-1,FH);
	
	if(index[strlen(index)-1] == er2)
	{
		index[strlen(index)-1] = er1;
	}
	strcpy(str,index);
}

FILE* OpenFile(char filename[], char mode[])
{
	FILE* FH = NULL;
	FH = fopen(filename,mode);
	while(FH == NULL)
	{
		printf("\nA File you entered does not exist. Exiting...\n");
		exit(0);
	}
	return FH;
}

void PrintETree(struct TreeNode*root, FILE* fp)
{
    char dollar = '$';
    char zero = '0';
    char one = '1';
	if(root != NULL)
	{
		if(root->character == dollar)
		{
			fprintf(fp,"%c", zero);
		}
		PrintETree(root->left, fp);
		PrintETree(root->right, fp);
		if (!(root->left) && !(root->right))
		{
			fprintf(fp, "%c",one);
			fprintf(fp,"%c",root->character);
		}
	}
}

void writeEncodeTree(struct TreeNode* root,int array[],int top, char ch,FILE*  filename)
{
	
	int i;
	if (root->left) 
    { 
        array[top] = 0; 
        writeEncodeTree(root->left, array, top + 1,ch,filename); 
    } 
  
    if (root->right) 
    { 
        array[top] = 1; 
        writeEncodeTree(root->right, array, top + 1,ch,filename); 
    } 
  
    if (!(root->left) && !(root->right)) //leaf
    { 
		if(ch == root->character)
		{
			for (i = 0; i < top; ++i)
			{
				fprintf(filename,"%d", array[i]);
			}
		}
    } 
}

int strCount(char str[], char ch)
{
	int count = 0;
	int i;
    char er1 = '\0';
	for(i = 0; str[i] != er1; i++)
	{
		if(ch == str[i])
		{
			count++;
		}
	}
	
	return count;
}

struct TreeNode* makeNode(char ch, int num)
{
	struct TreeNode * newNode = malloc(sizeof(struct TreeNode));
	newNode->right = NULL;
	newNode->left = NULL;
	newNode->character = ch;
	newNode->frequency = num;
	
	return newNode;
}

struct TreeQueue * makeQueue(int capacity)
{
	struct TreeQueue * QH = malloc(sizeof(struct TreeQueue));
	QH->start = -1;
	QH->end = -1;
	QH->capacity = capacity;
	QH->data = malloc(QH->capacity * sizeof(struct TreeNode));
	
	return QH;
}

struct TreeQueue* buildTree(char character[], int frequency[], int size) 
{ 
    struct TreeNode *left; 
	struct TreeNode *right;
	struct TreeNode *top;
    struct TreeQueue* Queue1  = makeQueue(size); 
    struct TreeQueue* Queue2 = makeQueue(size); 
	int i;
	int counter = -1;
    char dollar = '$';
    for (i = 0; i < size; ++i) 
	{
        enQueue(Queue1, makeNode(character[i], frequency[i]));
		counter++;
	}
	printf("\n");
    while (!((Queue1->start == -1) && (Queue2->start == Queue2->end && Queue2->start != -1))) 
    { 
        left = Min(Queue1, Queue2); 
        right = Min(Queue1, Queue2); 
        top = makeNode(dollar , left->frequency + right->frequency); 
		printf("buildHuffmanTree: (%c,%d)+(%c,%d)\t ->(%c,%d) ;\t N = %d\n",left->character,left->frequency,right->character,right->frequency,top->character,top->frequency,counter--);
        
		top->left = left; 
        top->right = right; 
        enQueue(Queue2, top); 
    } 
    return Queue2;
    //return deQueue(Queue2); 
} 

struct TreeNode* deQueue(struct TreeQueue* queue) 
{ 
    if (queue->start == -1)
	{
        return NULL;
	}
	
    struct TreeNode* tempNode = queue->data[queue->start]; 
	
    if (queue->start == queue->end)
	{
		queue->start = queue->end = -1; 
	}
    else
	{
        queue->start++; 
	}
    return tempNode; 
} 

void enQueue(struct TreeQueue* queue, struct TreeNode* item) 
{ 
    if (queue->end == queue->capacity -1) 
	{
		return;
	}
	
    queue->data[++queue->end] = item; 
	
    if (queue->start == -1) 
	{
        ++queue->start; 
	}
} 

struct TreeNode* Min(struct TreeQueue* Queue1,struct TreeQueue* Queue2)
{
    if (Queue1->start == -1) 
	{
        return deQueue(Queue2); 		
	}
	
    if (Queue2->start == -1) 
	{
        return deQueue(Queue1); 
	} 
	
    if (front(Queue1)->frequency < front(Queue2)->frequency) 
	{
        return deQueue(Queue1); 		
	}
  
    return deQueue(Queue2); 
}

struct TreeNode* front(struct TreeQueue* queue)
{
	if(queue->start == -1)
	{
		return NULL;
	}
	
	return queue->data[queue->start];
}

void displayTree(struct TreeNode* root,int array[],int top)
{
	int i;
	if (root->left) 
    { 
        array[top] = 0; 
        displayTree(root->left, array, top + 1); 
    } 
  
    if (root->right) 
    { 
        array[top] = 1; 
        displayTree(root->right, array, top + 1); 
    } 
  
    if (!(root->left) && !(root->right)) //leaf
    { 
        printf("(%c, ", root->character);  
		for (i = 0; i < top; ++i)
		{			
			printf("%d", array[i]); 
		}
		printf(")\n"); 
    } 
}
