	#include "math.h"
	#include <stdio.h>
	#include <stdlib.h>

	#define Error( Str )        FatalError( Str )
	#define FatalError( Str )   fprintf( stderr, "%s\n", Str ), exit( 1 )
	/* MUHAMMED ENES AKTURK - 150117036 
		there is printf statements for all the time and processes at line 417 and 429 
	*/
	#define Infinity (6000000000L)
	#define MaxTrees (12)   /* Stores 2^9 -1 items */
    #define Capacity (4095)

    //  #define MaxTrees (14)   /* Stores 2^14 -1 items */
    //  #define Capacity (16383)
		
	//	#define MaxTrees (30)   /* Stores 2^30 -1 items */
    //  #define Capacity (1073741823)

    struct BinNode;
    typedef struct BinNode *BinTree;
    struct Collection;
    typedef struct Collection *BinQueue;
	struct Node;
	typedef struct Node node ;
		
    BinQueue Initialize( void );
    void Destroy( BinQueue H );
    BinQueue MakeEmpty( BinQueue H );
    BinQueue Insert(node * Item, BinQueue H );
    double DeleteMin( BinQueue H );
    BinQueue Merge( BinQueue H1, BinQueue H2 );
    double FindMin( BinQueue H );
    int IsEmpty( BinQueue H );
    int IsFull( BinQueue H );

    typedef struct BinNode * Position ;
	typedef struct Node node ;
	BinQueue H1, H2;
		
	int currentTime = 0 , qValue=1 , valueOfExtrac ;
	node * root = NULL , * item2 = NULL;  			
	node * readInputFile(node * root , char * c);
	node * rewriteValues(node * root);
	int findEmax(node * root);
	int findTotalTime(node * root);
	node * calculatePriorityValues(node * root,int eMax);
	void printWaitingTime(node * root);
	
    struct BinNode
    {
		struct Node * Item;
        Position    LeftChild;
        Position    NextSibling;
    };

    struct Collection
    {
        int CurrentSize;
        BinTree TheTrees[ MaxTrees ];
    };
		
	struct Node
	{
		int Pid ;
		int firsteValue ;
		int eValue ;
		int tArr ;
		int tArrTemp ;
		int waitingTime ;
		int isinHeap ;
		double priorityValue ;
		struct Node * next ;
	};
		
		
    BinQueue Initialize( void ){
            BinQueue H;
            int i;

            H = malloc( sizeof( struct Collection ) );
            if( H == NULL )
                FatalError( "Out of space!!!" );
            H->CurrentSize = 0;
            for( i = 0; i < MaxTrees; i++ )
                H->TheTrees[ i ] = NULL;
            return H;
        }

    static void DestroyTree( BinTree T ){
            if( T != NULL )
            {
                DestroyTree( T->LeftChild );
                DestroyTree( T->NextSibling );
                free( T );
            }
        }

    void Destroy( BinQueue H ){
            int i;
            for( i = 0; i < MaxTrees; i++ ){
            	DestroyTree( H->TheTrees[ i ] );
			}
                
        }

    BinQueue MakeEmpty( BinQueue H ){
            int i;
            Destroy( H );
            for( i = 0; i < MaxTrees; i++ )
                H->TheTrees[ i ] = NULL;
            H->CurrentSize = 0;
            return H;
        }

    BinQueue Insert( node * Item, /*model_node_type m,*/ BinQueue H ){
            BinTree NewNode;
            BinQueue OneItem;
			int i;

            NewNode = malloc( sizeof( struct BinNode ) );
            if( NewNode == NULL )
                FatalError( "Out of space!!!" );
            NewNode->LeftChild = NewNode->NextSibling = NULL;
			NewNode->Item = Item;									// add the item into the heap 
			Item->isinHeap=1;										// change the value of isinHeap to 1 

            OneItem = Initialize( );
            OneItem->CurrentSize = 1;
            OneItem->TheTrees[ 0 ] = NewNode;
            return Merge( H, OneItem );
        }

    double DeleteMin( BinQueue H ){
            int i, j;
            int MinTree;   /* The tree with the minimum item */
            BinQueue DeletedQueue;
            Position DeletedTree, OldRoot ;
            double MinItem;
			//model_node_type MinItem;

            if( IsEmpty( H ) )
            {
                Error( "Empty binomial queue" );
				MinItem=-Infinity;
				
			//  for (i=0; i < max_act_const; i++) {
			//    MinItem.from[i].s=-1;
			//    MinItem->mn.from[i].a=m.from[i].a;
			//  }
                return MinItem;
            }

            MinItem = Infinity;
            for( i = 0; i < MaxTrees; i++ )
            {	
            	if( H->TheTrees[ i ] &&
                    H->TheTrees[ i ]->Item->priorityValue == MinItem && H->TheTrees[i]->Item->Pid < H->TheTrees[MinTree]->Item->Pid)
                {
            		MinItem = H->TheTrees[ i ]->Item->priorityValue;
                   	MinTree = i;
				}
		    	else if( H->TheTrees[ i ] &&
                    H->TheTrees[ i ]->Item->priorityValue < MinItem )
                {	
                    /* Update minimum */
                    MinItem = H->TheTrees[ i ]->Item->priorityValue;
                   	MinTree = i;                 
                }
            }
			
            DeletedTree = H->TheTrees[ MinTree ];
            
            DeletedTree->Item->isinHeap = 0 ;								// change the value of isinHeap in struct node the deleted item..
            valueOfExtrac = qValue ;										// value of extrac is for each element in heap , calculate the waiting time dependent on qValue and
            DeletedTree->Item->eValue-= qValue;								// e value of element .
            if(DeletedTree->Item->eValue < 0){
            	valueOfExtrac += DeletedTree->Item->eValue ;
			}            
            currentTime+=valueOfExtrac ;									// iterate currentTime for each element dependent on valueofExtrac
    		
            item2 = DeletedTree->Item ;										// item2 is the deleted item
            item2->tArrTemp += valueOfExtrac ;								
            if(item2->tArrTemp < currentTime){								// rewrite the tArrTemp for each element which one is selected 
            	item2->waitingTime += currentTime - item2->tArrTemp ;
            	item2->tArrTemp = currentTime ;
			}
			// the waiting time is calculated here	
			node * iter = root ;											// rewrite the tArrTemp and waitingTime's for each element which ones are waited in heap .
			while(iter!=NULL){
				if(iter->priorityValue != 0 && iter->Pid != item2->Pid){
				//	printf("selected item : P%d --- valueOfExtrac = %d --- waitingTime : %d\n\n",iter->Pid,valueOfExtrac,iter->waitingTime);
					iter->waitingTime +=valueOfExtrac ;
					iter->tArrTemp += valueOfExtrac ;
					if(iter->tArrTemp<currentTime){
						iter->waitingTime += currentTime-iter->tArrTemp;
						iter->tArrTemp = currentTime ;
					}
				}
				iter = iter->next ;
			}

        	//printf("selected item : P%d --- valueOfExtrac = %d --- waitingTime : %d --- tArrTemp : %d\n\n",item2->Pid,valueOfExtrac,item2->waitingTime,item2->tArrTemp);
            OldRoot = DeletedTree;
            DeletedTree = DeletedTree->LeftChild;
			
					
            free( OldRoot );

            DeletedQueue = Initialize( );
            DeletedQueue->CurrentSize = ( 1 << MinTree ) - 1;
            for( j = MinTree - 1; j >= 0; j-- )
            {
                DeletedQueue->TheTrees[ j ] = DeletedTree;
                DeletedTree = DeletedTree->NextSibling;
                DeletedQueue->TheTrees[ j ]->NextSibling = NULL;
            }
			
            H->TheTrees[ MinTree ] = NULL;
            H->CurrentSize -= DeletedQueue->CurrentSize + 1;
			
            Merge( H, DeletedQueue );
        
            return MinItem;
        }

    double FindMin( BinQueue H ){
            int i;
            double MinItem;
            if( IsEmpty( H ) )
            {
                Error( "Empty binomial queue" );
				MinItem=0;
                return MinItem;
            }
            MinItem = Infinity;
            for( i = 0; i < MaxTrees; i++ )
            {	
                if( H->TheTrees[ i ] &&
                            H->TheTrees[ i ]->Item->priorityValue < MinItem ) {
                    MinItem = H->TheTrees[ i ]->Item->priorityValue;
				}	

            } 
            return MinItem;
        }

    int IsEmpty( BinQueue H ){
            return H->CurrentSize == 0;
        }

    int IsFull( BinQueue H ){
            return H->CurrentSize == Capacity;
        }

    BinTree CombineTrees( BinTree T1, BinTree T2 ){
            if( T1->Item->priorityValue > T2->Item->priorityValue )
                return CombineTrees( T2, T1 );
                // If priority values are equal , make the root item which has less Pid 
            if( T1->Item->priorityValue == T2->Item->priorityValue && T1->Item->Pid > T2->Item->Pid){
            	return CombineTrees( T2 , T1 );
			}
            T2->NextSibling = T1->LeftChild;
            T1->LeftChild = T2;
            return T1;
        }

    BinQueue Merge( BinQueue H1, BinQueue H2 ){
            BinTree T1, T2, Carry = NULL;
            int i, j;
            if( H1->CurrentSize + H2->CurrentSize > Capacity )
                Error( "Merge would exceed capacity" );

            H1->CurrentSize += H2->CurrentSize;
            for( i = 0, j = 1; j <= H1->CurrentSize; i++, j *= 2 )
            {
                T1 = H1->TheTrees[ i ]; T2 = H2->TheTrees[ i ];
                switch( !!T1 + 2 * !!T2 + 4 * !!Carry )
                {
                    case 0: /* No trees */
                    case 1: /* Only H1 */
                        break;
                    case 2: /* Only H2 */
                        H1->TheTrees[ i ] = T2;
                        H2->TheTrees[ i ] = NULL;
                        break;
                    case 4: /* Only Carry */
                        H1->TheTrees[ i ] = Carry;
                        Carry = NULL;
                        break;
                    case 3: /* H1 and H2 */
                        Carry = CombineTrees( T1, T2 );
                        H1->TheTrees[ i ] = H2->TheTrees[ i ] = NULL;
                        break;
                    case 5: /* H1 and Carry */
                        Carry = CombineTrees( T1, Carry );
                        H1->TheTrees[ i ] = NULL;
                        break;
                    case 6: /* H2 and Carry */
                        Carry = CombineTrees( T2, Carry );
                        H2->TheTrees[ i ] = NULL;
                        break;
                    case 7: /* All three */
                        H1->TheTrees[ i ] = Carry;
                        Carry = CombineTrees( T1, T2 );
                        H2->TheTrees[ i ] = NULL;
                        break;
                }
            }
            return H1;
        }
		
	BinTree printTree(BinTree p, BinTree *r, int i){
		  BinTree t[20]={NULL}, q; int j;
		  for ( j=0; j<i; j++ ) t[j]= r[j];
		  i=0;
		  if (p!=NULL) { 
		    printf("& %2.3lf ",p->Item->priorityValue);
			q=p->NextSibling;
			j=0;
			do {
			  while (q!=NULL) { 
			   // printf("%2.3lf ",q->Item->priorityValue); 
			    if (q->LeftChild != NULL) { r[i]=q->LeftChild; i++; }
			    q=q->NextSibling;
			  }
			  q=t[j++];
			} while (q!=NULL);
		  }
		  else return NULL;
		  //for (j=0; j<i; j++) t[j]=NULL;
		  printf("\n");
		  printTree(p->LeftChild, r, i);
		}
	
	node * readInputFile(node * root , char * c){
		int pid , e , tarr ;
 		if(3 == sscanf(c,"%*[^0123456789]%d%*[^0123456789]%d%*[^0123456789]%d",&pid,&e,&tarr)){
			if(root==NULL){
				root=malloc(sizeof(node));
				root->Pid = pid ;
				root->firsteValue = e ;
				root->tArr = tarr;
				root->isinHeap = 0 ;
				root->next=NULL;
				return root ;				
			}
			else{
				node * iter = root ;
				while(iter->next!=NULL)
					iter=iter->next;
				iter->next=malloc(sizeof(node));
				iter->next->Pid=pid;
				iter->next->firsteValue=e;
				iter->next->tArr=tarr;
				iter->next->isinHeap= 0 ;
				iter->next->next=NULL;
				return root;
			}
		}
	}

	node * rewriteValues(node * root){
		node * iter = root ;
		while(iter!=NULL){
			iter->eValue = iter->firsteValue ;
			iter->waitingTime = 0 ;
			iter->isinHeap = 0 ;
			iter->priorityValue = 0 ;
			iter->tArrTemp = iter->tArr ;
			iter=iter->next;	
		}
		return root ;
	}
	
	int findEmax(node * root){
		int eMax = root->firsteValue ;
		node * iter = root ;
		while(iter!=NULL){
			if(iter->firsteValue > eMax)
				eMax=iter->firsteValue ;
			iter = iter->next;
		}
		return eMax ;
	}
	
	int findTotalTime(node * root){
		node * iter = root ;
		int totalTime = 0;
		while(iter!=NULL){
			totalTime += iter->firsteValue ;
			iter=iter->next ;
		}
		return totalTime;
	}
	
	node * calculatePriorityValues(node * root , int eMax){
		node * iter = root ;
		while(iter!=NULL){
			iter->priorityValue=0;
			if(iter->tArr <= currentTime && iter->eValue > 0){
				if(iter->firsteValue == iter->eValue)
					iter->priorityValue = iter->eValue ;
				else
					iter->priorityValue = iter->eValue*(1.0/(exp((-1)*pow(((2.0*iter->eValue)/(3*eMax)),3))));
			}
			iter=iter->next;	
		}
		iter = root ;
		node * item = NULL ;
		while(iter!=NULL){
			if(iter->priorityValue!=0){
//				printf("%d\t\tP%d\t\t\tP%d : %lf\n",currentTime,iter->Pid,iter->Pid,iter->priorityValue);
				if(!iter->isinHeap)
					Insert(iter,H1);
				if(item==NULL){
					item = iter ;
				}
				else if(iter->priorityValue < item->priorityValue){
					item = iter ;
				}
			}
			iter=iter->next;
		}
//		printf("\n");
		return item ;
	}

	void printWaitingTime(node * root){
		printf("q=%d\n\nWaiting Time\n-------------\n",qValue);
		node * iter = root ;
		int totalAWT = 0 , totalItem = 0;
		while(iter!=NULL){
			totalItem++;
			totalAWT += iter->waitingTime  ;
			printf("P%d : %d\n",iter->Pid,iter->waitingTime);
			iter = iter->next;
		}
		printf("\nAWT = %d/%d = %.3lf\n\n",totalAWT,totalItem,totalAWT*1.0/totalItem);
	}
	
				
	main(){	
		BinTree p, r[20]={NULL};
		H1 = Initialize( );
		int eMax , q , totalTime ;
		char c[25];
		FILE * fp ;
		fp = fopen("input.txt","r");
		if(fp==NULL){
			printf("File is not exist.");
			return 0 ;
		}
		else{
			while(fgets(c,sizeof(c),fp)){
		  	if(c[0]=='P')
		  		root = readInputFile(root , c);
			}
		}
		eMax = findEmax(root);
		totalTime = findTotalTime(root);
		q=10 ;
		root=rewriteValues(root);
		int i ;
		for(;qValue<=q ; qValue++){
			printf("\nq : %d\n\nTime\tProcesses in BH\t\tPriority Value of Processes in BH\n----------------------------------------------------------------\n",qValue);
			for(currentTime=0 ; currentTime<totalTime ;){
				node * item = NULL ;
				item=calculatePriorityValues(root,eMax);
				DeleteMin(H1);
			}
			printWaitingTime(root);
			root=rewriteValues(root);
		}
	}                                                  

	
