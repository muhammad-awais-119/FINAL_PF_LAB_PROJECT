#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

char password[30],count=1;
char login[25];
char acc[14];
char iban[24]={"PK00CLIB00"};

void History(float send,int receiver_index){
    time_t now;
    time(&now); // Get current time as seconds since epoch
    struct tm *local = localtime(&now); // Convert to local time structure
    char user[20],receiver[20],myline[256],buffer[25];
    char username[256]="";
    char receivername[256]="";
    FILE *ptr=NULL;
    ptr=fopen("accdetails.txt","r");
    //fetching user account
    for (int i = 1; i < count; i++)
    {
        fgets(myline,256,ptr);
    }
    for (int i = 1; i <= 5; i++)
    {
        if (i==1||i==2)
        {
            fscanf(ptr,"%s",user);
            strcat(username,user);
        }
        else
        {
            fscanf(ptr,"%s",user);
        }
    }
    fclose(ptr);
    ptr=fopen("accdetails.txt","r");
    for (int i = 1; i < receiver_index; i++)
    {
        fgets(myline,256,ptr);
    }
    for (int i = 1; i <= 5; i++)
    {
        if (i==1||i==2)
        {
            fscanf(ptr,"%s",receiver);
            strcat(receivername,receiver);
        }
        else
        {
            fscanf(ptr,"%s",receiver);
        }
        
    }
    fclose(ptr);
    strcat(user,".txt");
    strcat(receiver,".txt");
    //user transaction
    ptr=fopen(user,"a");
    sprintf(buffer, "%.2f", send);
    fputs("\033[1;32m", ptr);
    fputs(buffer, ptr);
    fputs("\033[0m", ptr);
    fputs(" RS ",ptr);
    fputs("\033[33mTRANSFERRED\033[0m TO ", ptr);
    fputs("\033[1;96m", ptr);
    fputs(receivername, ptr);
    fputs("\033[0m", ptr);
    fputs(" ",ptr);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local);
    fputs(" at ",ptr);
    fputs("\033[1;38;5;216m", ptr);
    fputs(buffer, ptr);
    fputs("\033[0m", ptr);
    fputs("\n",ptr);
    fclose(ptr);
    //recevier transaction
    ptr=fopen(receiver,"a");
    sprintf(buffer, "%.2f", send);
    fputs("\033[1;32m", ptr);
    fputs(buffer, ptr);
    fputs("\033[0m", ptr);
    fputs(" RS ",ptr);
    fputs("\033[32mRECEIVED\033[0m FROM ", ptr);
    fputs("\033[1;96m", ptr);
    fputs(username, ptr);
    fputs("\033[0m", ptr);
    fputs(" ",ptr);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local);
    fputs(" at ",ptr);
    fputs("\033[1;38;5;216m", ptr);
    fputs(buffer, ptr);
    fputs("\033[0m", ptr);
    fputs("\n",ptr);
    fclose(ptr);
    
}

void display_balance(){
    char mycash[256];
    FILE *ptr=NULL;
    ptr=fopen("accountcash.txt","r");
    for (int i = 1; i <= count; i++)
    {
        fscanf(ptr,"%s",mycash);
    }
    printf("\033[96m\t\t\tYOUR CURRENT BALANCE IS \033[0m");
    printf("\033[32m%s\033[96m RS\033[0m", mycash);
    printf("\n");
    fclose(ptr);
}

void transfercash(int receiver_index,int send){
    char mycash[25],receiver_cash[25],buffer[25];
    float ucash,rcash;
    int c=1;
    FILE *ptr=NULL;
    ptr=fopen("accountcash.txt","r");
    for (int i = 1; i <= count; i++)//to get user cash from the file
    {
        fscanf(ptr,"%s",mycash);
    }
    //printf("%s",mycash);
    fclose(ptr);
    ptr=fopen("accountcash.txt","r");
    for (int i = 1; i <= receiver_index; i++)//to get receiver cash from the file
    {
        fscanf(ptr,"%s",receiver_cash);
    }
    fclose(ptr);
    ucash=atof(mycash);
    rcash=atof(receiver_cash);
    if (ucash<send)
    {
      printf("\033[1;31m\nNOT ENOUGH CASH AVAILABLE IN YOUR ACCOUNT\033[0m");
    }
    else
    {
        ucash=ucash-send;
        rcash=rcash+send;
        FILE *ptr=NULL;
        FILE *fptr=NULL;
        ptr=fopen("accountcash.txt","r");
        fptr=fopen("temp.txt","w");
        while (fscanf(ptr,"%s",mycash)!=EOF)
        {
            if (c==receiver_index)
            {
               sprintf(buffer, "%.2f", rcash);
               fputs(buffer,fptr);
               fputs("\n",fptr);
            }else if (c==count)
            {
               sprintf(buffer, "%.2f", ucash);
               fputs(buffer,fptr);
               fputs("\n",fptr);
            }
            else
            {
               fputs(mycash,fptr);
               fputs("\n",fptr);
            }
            c++;
        }
        fclose(ptr);
        fclose(fptr);
        remove("accountcash.txt");
        rename("temp.txt","accountcash.txt");
        printf("\033[1;32m\ncash transferred successfully\033[0m");
        History(send,receiver_index);

    }
}

int authenticate(char IDR[],char PH[]){
    char ID[100];
    char phone[100];
    FILE *ptr=NULL;
    ptr=fopen("accdetails.txt","r");
    while (fscanf(ptr,"%s",ID)!=EOF)
    { 
    for (int i = 2; i <= 3; i++)
    {
        fscanf(ptr,"%s",ID);
    }
    fscanf(ptr,"%s",phone);
    if (strcmp(ID,IDR)==0 || strcmp(phone,PH)==0)
    {
        printf("\033[31m\nYOU HAVE ENTERED AN ID OR PHONE NUMBER THAT IS ALREADY REGISTERED\033[0m");
        fclose(ptr);
        return 1;
    }
    fscanf(ptr,"%s",phone);
    fscanf(ptr,"%s",phone);
    }
    fclose(ptr);
    return 0;
}


void userdetails(){
    int x=1,y=1;
    char firstname[25],lastname[25],ID[25],phone[25];
    
    while (x==1)
    {
       x=0;
       printf("\033[38;5;216m\nENTER YOUR FIRST NAME: \033[0m");
       fgets(firstname,25,stdin);
       firstname[strcspn(firstname, "\n")] = '\0';
       if (strlen(firstname)==0)
       {
        printf("\033[31m\nname cannot be empty\033[0m");
        printf("\033[31m\nre enter \033[0m");
        x=1;
       }else {
       
       for (int i = 0; i < strlen(firstname); i++)
       {
        if (firstname[i]>='0' && firstname[i]<='9')
        {
            printf("\033[31m\nNAME CANNOT HAVE ANY NUMBER IN IT\033[0m");
            printf("\033[31m\nRE ENTER \033[0m");
            x=1;
            break;
        }
        else if (!((firstname[i] >= 'A' && firstname[i] <= 'Z') ||
                   (firstname[i] >= 'a' && firstname[i] <= 'z') ))
        {
            printf("\033[31m\nNAME CANNOT HAVE ANY SPECIAL CHARACTERS OR SPACES\033[0m");
            printf("\033[31m\nRE ENTER \033[0m");
            x=1;
            break;
        }
       }
       }
    }
    x=1;
    while (x==1)
    {
        x=0;
        printf("\033[38;5;216m\nENTER YOUR LAST NAME (WITHOUT ANY GAPS): \033[0m");
        fgets(lastname,25,stdin);
        lastname[strcspn(lastname, "\n")] = '\0';
        if (strlen(lastname)==0)
       {
        printf("\033[31m\nNAME CANNOT BE EMPTY\033[0m");
        printf("\033[31m\nRE ENTER \033[0m");
        x=1;
       }
       else 
       {
       for (int i = 0; i < strlen(lastname); i++)
       {
        if (lastname[i]>='0' && lastname[i]<='9')
        {
            printf("\033[31m\nNAME CANNOT HAVE ANY NUMBER IN IT\033[0m");
            printf("\033[31m\nRE ENTER \033[0m");
            x=1;
            break;
        }
        else if (!((lastname[i] >= 'A' && lastname[i] <= 'Z') ||
                   (lastname[i] >= 'a' && lastname[i] <= 'z') ))
        {
            printf("\033[31m\nNAME CANNOT HAVE ANY SPECIAL CHARACTERS OR SPACES\033[0m");
            printf("\033[31m\nRE ENTER \033[0m");
            x=1;
            break;
        }
       }
       }
    }
    while (y==1){
    x=1;
    while (x==1)
    {
        x=0;
        printf("\033[38;5;216m\nENTER YOUR ID: \033[0m");
        fgets(ID,25,stdin);
        ID[strcspn(ID, "\n")] = '\0';
        if (strlen(ID)<13||strlen(ID)>13)
        {
            printf("\033[31m\nINVALID ID FORMAT\033[0m");
            printf("\033[31m\nRE ENTER YOUR ID \033[0m");
            x=1;
        }
        else
        {
            for (int i = 0; i < 13; i++)
            {
                if (!(ID[i]>='0' && ID[i]<='9'))
                {
                   printf("\033[31m\nINVALID ID FORMAT\033[0m");
                   printf("\033[31m\nRE ENTER YOUR ID \033[0m");
                   x=1;
                   break;
                }
            }  
        }   
    }
    x=1;
    while (x==1)
    {
        x=0;
        printf("\033[38;5;216m\nENTER YOUR PHONE NUMBER: \033[0m");
        fgets(phone,25,stdin);
        phone[strcspn(phone, "\n")] = '\0';
        if (strlen(phone)<11||strlen(phone)>11)
        {
            printf("\033[31m\nINVALID PHONE NUMBER FORMAT [0332..]\033[0m");
            printf("\033[31m\nRE ENTER YOUR PHONE NUMBER \033[0m");
            x=1;
        }
        else
        {
            for (int i = 0; i < 11; i++)
            {
                if (!(phone[i]>='0' && phone[i]<='9'))
                {
                   printf("\033[31m\nINVALID PHONE NUMBER FORMAT [0332..]\033[0m");
                   printf("\033[31m\nRE ENTER YOUR PHONE NUMBER \033[0m");
                   x=1;
                   break;
                }
            }  
        }
    }
    int z=authenticate(ID,phone);
    if (z==0)
    {
        y=0;
        FILE *fptr=NULL;
    fptr=fopen("accdetails.txt","a");
    fputs(firstname,fptr);
    fputs(" ",fptr);
    fputs(lastname,fptr);
    fputs(" ",fptr);
    fputs(ID,fptr);
    fputs(" ",fptr);
    fputs(phone,fptr);
    fclose(fptr);
    }
    else
    {
       printf("\033[31m\nRE ENTER YOUR ID AND PHONE NUMBER \033[0m");
    }
    } 
}


void acc_numGenerator(){
    srand(time(NULL));
    for (int i = 0; i < 14; i++)
    {
        acc[i]=(rand()%10+'0');
    }
        FILE *ptr=NULL;
        ptr=fopen("accdetails.txt","a");
        fputs(" ",ptr);
        fputs(acc,ptr);
        fputs(" ",ptr);
        fclose(ptr);
}

void IBAN_generator(){
    strcat(iban,acc);
    FILE *ptr=NULL;
    ptr=fopen("accdetails.txt","a");
    fputs(iban,ptr);
    fputs("\n",ptr);
    fclose(ptr);
}

void display_accDetails(int index){
    char myline[256];
    char headings[6][256]={{"FIRST NAME: "}
                          ,{"last name: "}
                          ,{"ID NUMBER: "}
                          ,{"PHONE NUMBER: "}
                          ,{"ACCOUNT NUMBER: "}
                          ,{"IBAN NUMBER: "}};
    FILE *ptr=NULL;
    ptr=fopen("accdetails.txt","r");
    for (int i = 1; i < index; i++)
    {
        fgets(myline,256,ptr);
    }
    int row=0;
    fscanf(ptr,"%s",myline);
    for (int i = 1; i <= 6; i++)
    {
        printf("\033[96m\t\t\t%s\033[0m", headings[row]);
        row++;
        printf("%s",myline);
        printf("\n");
        fscanf(ptr,"%s",myline);
    }
    fclose(ptr);
}



void newAccount_registration(char string[],char pass[]){
    FILE *ptr=NULL;
    ptr=fopen("account.txt","a");
    if (ptr!=NULL){
        fputs(string,ptr);
        fputs("\n",  ptr);
        fputs(pass,  ptr);
        fputs("\n",ptr);
    }
    fclose(ptr);
}

int acc_index(char accountnumber[]){
    int index=1;
    char myline[100];
    FILE *ptr=NULL;
    ptr=fopen("accdetails.txt","r");
    if (ptr!=NULL)
    {
        while(fscanf(ptr,"%s",myline)!=EOF){   
            for (int i = 1; i <= 3; i++)
            {
                fscanf(ptr,"%s",myline);
            }
            fscanf(ptr,"%s",myline);
            if (strcmp(accountnumber,myline)==0)
            {
                fclose(ptr);
                return index;
            }
            else
            {
                fscanf(ptr,"%s",myline);
                index++;
            }
        }
        fclose(ptr);
        printf("\033[31m\nACCOUNT DOES NOT EXIST OR YOU HAVE ENTERD INCORRECTLY\033[0m");
        return -1;
    } 
}

int passStrength(){
    int count_capital=0,count_small=0,count_num=0,count_special=0,strength=0;
    char single;
    if (strlen(password)<12)
    {
        printf("\033[31m\nPASSWORD MUST BE GREATER THAN OR EQUAL TO 12 CHARACTERS IN LENGTH\033[0m");
        return 1;
    }
    for (int i = 0; i < strlen(password); i++)
    {
        single=password[i];
        //printf("%c\n",single);
        if (single>='A' && single<='Z')
        {
            count_capital++;
            strength=strength+2; // 2 points for capital letter
        }
        else if (single>='a' && single<='z')
        {
            count_small++;
            strength=strength+1; // 1 point for small letter
        }
        else if (single>='0' && single<='9')
        {
            count_num++;
            strength=strength+3; // 3 points for number
        }
        else
        {
            count_special++;
            strength=strength+5; // 5 points for special character
        } 
    }
    if (count_capital<2)
    {
       printf("\033[31m\nYOUR PASSWORD MUST INCLUDE AT LEAST TWO CAPITAL CHARACTERS\033[0m");
        return 1;
    }
    if (count_small<2)
    {
        printf("\033[31m\nYOUR PASSWORD MUST INCLUDE AT LEAST TWO SMALL CHARACTERS\033[0m");
        return 1;
    }
    if (count_num<2)
    {
        printf("\033[31m\nYOUR PASSWORD MUST INCLUDE AT LEAST TWO NUMBER CHARACTERS\033[0m");
        return 1;
    }
    if (count_special<3)
    {
        printf("\033[31m\nYOUR PASSWORD MUST INCLUDE AT LEAST 3 SPECIAL CHARACTERS\033[0m");
        return 1;
    }
    printf("\033[38;5;216mPASSWORD STRENGTH SCORE IS \033[96m%d\033[0m", strength);
    if (strength<30)
    {
       printf("\033[31m\nPASSWORD IS WEAK\033[0m");
       return 1;
    }
    else
    {
       printf("\033[32m\nPASSWORD IS STRONG\033[0m");
    }
}

int check_accDetails(){
    char myline[100];
    char pass[25];
    count=1;// for tracking the user index in the file
    FILE *ptr=NULL;
    ptr=fopen("account.txt","r");
    if (ptr!=NULL)
    {
        while (fscanf(ptr,"%s",myline)!=EOF)
        {
            fscanf(ptr,"%s",pass);
            if (strcmp(login,myline)==0)
            {
                if (strcmp(password,pass)==0)
                {
                    printf("\033[32m\nLOGIN SUCCESSFUL\033[0m");
                    fclose(ptr);
                    return 0;
                }
                else
                {
                    printf("\033[31m\nLOGIN FAILED\033[0m");
                    fclose(ptr);
                    return 1;
                }
            }
            else
            {
                count++;
            }
        }
        printf("\033[31m\nLOGIN FAILED\033[0m");
        fclose(ptr);
        return 1;
    }
    fclose(ptr);   
}

int check_acc_exsists(){
    char myline[100];
    char pass[25];
    FILE *ptr=NULL;
    ptr=fopen("account.txt","r");
    if (ptr!=NULL)
    {
        while (fscanf(ptr,"%s",myline)!=EOF)
        {
            fscanf(ptr,"%s",pass);
            if (strcmp(login,myline)==0)
            {
                printf("\033[31m\nUSERNAME HAS ALREADY BEEN TAKEN\033[0m");
                fclose(ptr);
                return 1;
            }
        }
        printf("\033[32m\nVALID USERNAME\033[0m");
        fclose(ptr);
        return 0;
    }
    fclose(ptr);   
}

void bring_info(int index){
    FILE *ptr=NULL;
    char mydata[256],myword[25];
    char headings[3][256]={{"FIRST NAME: "},{"LAST NAME: "},{"CNIC: "}};
    ptr=fopen("accdetails.txt","r");
    printf("====================================================================\n");
    printf("\n");
    for (int i = 1; i <= index-1; i++)
    {
        fgets(mydata,256,ptr);
    }
    for (int i = 0; i < 3; i++)
    {
        fscanf(ptr,"%s",myword);
        printf("\033[34m\t\t\t%s\033[0m", headings[i]);
        printf("%s",myword);
        printf("\n");
    }
    printf("\n");
    printf("====================================================================\n");
    fclose(ptr);
}

int main (){
    int x=0;
    float cash;
    char myline[256];
    char myacc[20];
    char decision[25];
    int check, attempts=1,succ=0;
    printf("\033[96m");
printf("\n");
printf("\t\t====================================================\n");
printf("\t\t                   CLI BANKING\n");
printf("\t\t====================================================\n");
printf("\033[38;5;216m");
printf("\033[38;5;216m");
printf("\n\t\t   BANKING MADE SIMPLE, BANKING MADE BETTER.\n");

printf("\033[0m");
    while (x==0)
{
    printf("\033[1;32m\nENTER 1 FOR NEW ACCOUNT REGISTRATION OR 0 FOR LOGIN OR /\033[1;31mEND\033[1;32m TO \033[1;31mEXIT\033[1;32m THE PROGRAM: \033[0m");    scanf("%s",decision);
    getchar(); // removes the leftover newline
    while (strcmp(decision,"1")!=0 && strcmp(decision,"0")!=0 && strcmp(decision,"/END")!=0)
    {
        printf("\033[31m\nplease follow the given command properly\033[0m");
        printf("\033[32m\nenter 1 for new account registration or 0 for login or /\033[31mEND\033[32m to \033[31mexit\033[32m the program: \033[0m");
        scanf("%s",decision);
        getchar();
    }
  if (strcmp(decision,"/END")==0) {break;}  
  if (strcmp(decision,"1")==0)
  {
    printf("\033[38;5;216mYOU WILL NOW NEED TO ENTER YOUR PERSONAL DETAILS FOR REGISTRATION\n\033[0m");
    userdetails();
    printf("\033[32m\nYOUR PERSONAL INFO WAS SUCCESSFULLY ADDED IN OUR DATABASE!! \033[0m");
    printf("\033[38;5;216m\nENTER AN ACCOUNT NAME FOR YOUR NEW BANK ACCOUNT: \033[0m");
    scanf("%s",login);
    while (check_acc_exsists()==1)
    {
        printf("\033[31m\nRE ENTER USERNAME: \033[0m");
        scanf("%s",login);
    }
    printf("\033[33m\nPASSWORD MUST BE >=\033[32m12\033[33m CHARACTERS IN LENGTH\033[0m");
    printf("\033[33m\nMUST INCLUDE \033[32m2\033[33m CAPITAL LETTERS\033[0m");
    printf("\033[33m\n\033[32m2\033[33m SMALL LETTERS\033[0m");
    printf("\033[33m\n\033[32m2\033[33m NUMBERS\033[0m");
    printf("\033[33m\n\033[32m3\033[33m SPECIAL CHARACTERS\033[0m");
    printf("\033[38;5;216m\nENTER A STRONG PASSWORD FOR YOUR ACCOUNT: \033[0m");
    scanf("%s",password);
    while (passStrength()==1)
    {
        printf("\033[31m\nRE ENTER: \033[0m");
        scanf("%s",password);
    }
    acc_numGenerator();
    IBAN_generator();
    newAccount_registration(login,password);
    printf("\033[1;32m\nACCOUNT REGISTRATION COMPLETE\033[0m");
    printf("\033[33m\nYOUR ACCOUNT NUMBER: \033[32m%s\033[0m", acc);
    printf("\033[33m\nYOUR IBAN NUMBER: \033[32m%s\033[0m", iban);
    FILE *ptr=NULL;
    ptr=fopen("accountcash.txt","a");// initialising cash for new accounts
    fputs("1000\n",ptr);// free 1000 rupees for opening an account
    fclose(ptr);
    
  }
  else if (strcmp(decision,"0")==0)
  {
    x=1;
    printf("\033[38;5;216m\nENTER YOUR LOGIN DETAILS\n\033[0m");
    printf("\033[38;5;216m\nENTER USER ACCOUNT: \033[0m");
    scanf(" %s",login);
    printf("\033[38;5;216m\nENTER PASSWORD: \033[0m");
    scanf("%s",password);
    printf("\n");
    check=check_accDetails();

    while (check==1 && attempts<=3)
   {
        attempts++;
        printf("\033[31m\nENTER YOUR USER ID AND LOGIN PASSWORD CORRECTLY !\033[0m");
        printf("\033[38;5;216m\nENTER YOUR LOGIN DETAILS: \033[0m");
        printf("\033[38;5;216m\nENTER USER ACCOUNT: \033[0m");
        scanf(" %s",&login);
        printf("\033[38;5;216m\nENTER PASSWORD: \033[0m");
        scanf("%s",password);
        check=check_accDetails();
   }
   if (attempts==3){printf("\033[31m\nDEVICE IS RESTRICTED FOR 24 HOURS\033[0m");strcpy(decision,"/END");break;}
   else
   {
    succ=1;
   }
  }
}

int command=0;
if (strcmp(decision,"/END")==0)
{
    printf("\033[1;32m\n\t\t\tTHANK YOU FOR USING OUR SERVICES\033[0m");
}
else if(succ==1)
{
    x=0;
    while (x==0)
    {
        printf("\n");
        printf("\n********************************************************************\n");
        printf("\033[96m\t\t\tfollowing commands are available:\n\033[0m");
        printf("\n");
        printf("\033[32m\t\t\t1. \033[33mDISPLAY USER DETAILS\033[0m\n");
        printf("\033[32m\t\t\t2. \033[33mTRANSFER MONEY\033[0m\n");
        printf("\033[32m\t\t\t3. \033[33mDISPLAY BALANCE\033[0m\n");
        printf("\033[32m\t\t\t4. \033[33mTRANSACTION HISTORY\033[0m\n");
        printf("\033[32m\t\t\t5. \033[33mTO END THE PROGRAM\033[0m\n");
        printf("\n");
        printf("********************************************************************\n");
        printf("\033[34m\nENTER COMMAND NUMBER:  \033[0m");
        scanf("%d",&command);
        printf("\n");
        if (command==1)
        {
           printf("====================================================================\n");
           display_accDetails(count);
           printf("====================================================================\n");
        }
        else if (command==2)
        {
            printf("\033[38;5;216m\nENTER THE ACCOUNT NUMBER OF THE RECEIVER OR \033[31m-1\033[38;5;216m TO \033[31mEND\033[38;5;216m TRANSACTION PROCESS:  \033[0m");            scanf("%s",acc);
            if (acc_index(acc)==-1)
            {
                x=0;
            }
            else if (acc_index(acc)==count)// check
            {
               printf("\033[31m\nYOU CANNOT SEND MONEY TO YOUR SELF !!\033[0m");
            }
            else
            {
                printf("\033[38;5;216m\nverify the details of the receiver:\n\033[0m");
                bring_info(acc_index(acc));
                printf("\033[38;5;216m\nenter amount to send or \033[31m-1\033[38;5;216m to \033[31mend\033[38;5;216m transfer \033[0m");
                scanf("%f",&cash);
                if (cash==-1)
                {
                    break;
                }
                while (cash<=0)
                {
                    printf("\033[38;5;216m\nyou cannot send \033[31m0\033[38;5;216m or \033[31mnegetive\033[38;5;216m rupees re enter [enter \033[31m-1\033[38;5;216m to \033[31mexit\033[38;5;216m transfer]: \033[0m");
                    scanf("%f",&cash);
                    if (cash==-1.0)
                    {
                        break;
                    }
                }
                if (cash!=-1)
                {
                    if (acc_index(acc)!=count)
                    {
                        transfercash(acc_index(acc),cash);
                    }
                }
            } 
        }
        else if (command==3)
        {
            printf("====================================================================\n");
            printf("\n");
            display_balance();
            printf("\n");
            printf("====================================================================\n");
        }
        else if (command==4)
        {
            printf("====================================================================\n");
            printf("\n");
            FILE *ptr=NULL;
            ptr=fopen("accdetails.txt","r");
            for (int i = 1; i < count; i++)
            {
                fgets(myline,256,ptr);
            }
            for (int i = 1; i <=5 ; i++)
            {
                fscanf(ptr,"%s",myacc);
            }
            strcat(myacc,".txt");
            fclose(ptr);
            FILE *fptr=NULL;
            fptr=fopen(myacc,"r");
            if (fptr!=NULL)
            {
                while (fgets(myline,256,fptr))
                {
                    printf("\t%s\n",myline);
                }
            }
            else
            {
                printf("\n");
                printf("\033[31m\t\t\tNO TRANSACTIONS HAVE OCCURED\033[0m");
                printf("\n");
            }    
            printf("\n");
            printf("====================================================================\n");
        }
        else if (command==5)
        {
           x=1;
          printf("\033[1;32m\n\t\t\tTHANK YOU FOR USING OUR SERVICES\033[0m");
        }
        else
        {
           printf("\033[31m\nINVALID COMMAND\033[0m");
        } 
    }
    printf("\nTHE END");
}
}