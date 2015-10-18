struct tnode{
    char c;
    int count;
    struct tnode *next;
    struct tnode *left;
    struct tnode *right;
};
struct node{
    char *code;
    char ch;
    struct node* next;
};
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#define MAX 100
struct tnode *addtree(struct tnode *,char );
void print_tnode(struct tnode *);
void sortedtree(struct tnode **);
void sortedinsert(struct tnode ** , struct tnode*);
void buildtree(struct tnode **);
void splittree(struct tnode*, struct tnode** ,struct tnode**);
int node_len(struct tnode *);
struct tnode*  combi_freq(struct tnode *);
void print_tree(struct tnode *);
void assign_codes(struct tnode *, struct node **,char *,int );
char *pattern(char *,int,char);
void push(char ,struct node **, char *);
void print_node(struct node *);
void encode(struct node *, char *,char *);
void decode(struct tnode *, char * ,char *);
main()
{
// "aaabccdeeeeeffg"
    char pat[MAX] = " ";
    char en_msg[MAX] = "";
    char de_msg[MAX] = "";
    char word[MAX] = "aaabccdeeeeeffg";
    struct tnode *root;
    struct node *code = NULL;
    root =NULL;
    int i;
    for(i=0; word[i] != '\0'; i++){
	root = addtree(root,word[i]);
    }
    
    printf("\nword frequency is\n");
    print_tnode(root);

    sortedtree(&root);
    printf("\nfrequency in sorted order is \n ");
    print_tnode(root); 


    buildtree(&root);
    printf("\n Tree structure of word frequency\n");
    print_tree(root); 


    assign_codes(root,&code,pat,0);  
    printf("\n Code assigned in corresponding letters is\n");
    print_node(code);


    encode(code ,word,en_msg);
    printf("\n  Encoded form of given string is\n");
    printf("%s \n",en_msg);
    

    decode(root,en_msg,de_msg);
    printf("\n  Decoded string is\n");
    printf("%s \n\n",de_msg);
	
}


struct tnode *addtree(struct tnode *p,char c)
{
        
    if(p == NULL){
	p = (struct tnode*) malloc(sizeof(struct tnode)) ;
	p->c = c;
        p->count = 1;
	p->next = NULL;
	p->left = NULL; 
	p->right = NULL;
    }else if(p->c == c)
	p->count++;
     else
	p->next = addtree(p->next,c);
    return p;
}


void print_tnode(struct tnode *p)
{
	while(p != NULL){
	      printf("%c : %d \n ",p->c,p->count);
	      p = (p->next);
        }
}


void sortedinsert(struct tnode** head, struct tnode* temp)

{
     if(*head == NULL || (*head)->count >= temp->count){
	  temp->next = *head;
	  *head = temp;
     }else{
	  struct tnode *current = *head;
	  while(current->next != NULL && (current->next->count < temp->count) )
	 	current = current->next;
	  temp->next = current->next;
	  current->next = temp;
     }
}


void sortedtree(struct tnode** head)
{
    struct tnode* temp = NULL;
    struct tnode* nextt ;
    struct tnode* current = *head;
    while(current != NULL){
	nextt = current->next;
        sortedinsert(&temp,current);
	current =    nextt;
    }
    *head = temp;
}


void buildtree(struct tnode **head)
{
     struct tnode* temp = *head;
     struct tnode *leasttwo,*theRest;
     struct tnode *newnode;
     while(node_len(*head) > 1){

        splittree(*head,&leasttwo,&theRest);	
        newnode = combi_freq(leasttwo);
	newnode->next = theRest;
	*head =(theRest, newnode);
        sortedtree(head);
    }
   
}


int node_len(struct tnode *p)
{
     int n = 0;
     while(p != NULL){
	n = n+1;
   	p = p->next;
     }
     return n;
}


	
void splittree(struct tnode * head,struct tnode **leasttwo, struct tnode **rest)
{
     struct tnode* temp = head;
     *rest = temp->next->next;
     temp->next->next = NULL;
     *leasttwo = temp;
}

 
struct tnode *combi_freq(struct tnode *leasttwo)
{
     struct tnode *new = (struct tnode*) malloc(sizeof(struct tnode));
     new->count = leasttwo->count +leasttwo->next->count;
     new->c = '\0';
     new->left = leasttwo;
     new->right = leasttwo->next;
     new->next = NULL;
     return new;
}


void print_tree(struct tnode *p )
{
    if(p != NULL){
          print_tree(p->left);
	  printf("%d , %c\n",p->count,p->c );
          print_tree(p->right);
    }
}



void assign_codes(struct tnode *head, struct node **code,char *pat,int i)
{
     if(head->left == NULL || head->right == NULL){
            push(head->c,code,pattern(pat,i,'\0'));
	}else{
              assign_codes(head->left,code,pattern(pat,i,'0'),i+1);
	      assign_codes(head->right,code,pattern(pat,i,'1'),i+1);
	}


}



char *pattern(char *s, int l, char c)
{
       *(s+l) = c;
       return s;
}



void push (char c ,struct node **p, char *w )
{
     struct node *new = (struct node*) malloc(sizeof(struct node));
     new->code = strdup(w);
     new->ch = c;
     new->next =*p;
     *p = new;
}


   
void print_node(struct node *p)
{
     while(p != NULL){
         printf("%s  %c \n", p->code,p->ch);
	 p = p->next;
     }
}


void encode(struct node *code, char *word,char *e_msg)
{
     char ch[MAX];
     struct node* dup;
     for( ;*word != '\0';word++ ){
 	  dup = code;
	  while(dup != NULL){
	       if(dup->ch == *word){
			strcat(e_msg,dup->code);
			break;
		}
		dup = dup->next;
	   }
      }
}


void decode(struct tnode *head,char *e_msg, char *de_msg)
{
     char * msg;
     struct tnode* dup = head;
     for(msg = e_msg; *msg !='\0' ;msg++){
	if(*msg == '0')
	     dup = dup->left;
	else
	     dup = dup->right;
 	if(dup->left == NULL || dup->right == NULL){
	     *(de_msg++) = dup->c;
	     dup = head;
	}
     }
     *de_msg = '\0';
}
	
	
