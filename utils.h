#define STR_MAX_LENGHT 50
#define FILE_NAME "logindata.txt"      

enum status{
	FAUX,
	VRAI
};

void mainMenu();
void login();
void addUser();
char get_class(int ip1);
int verify(int ip1, int ip2, int ip3, int ip4, int ipERR);
int verifyMSK(int msk1, int msk2, int msk3, int msk4, int mskERR);
char getMSKclass (int msk1, int msk2, int msk3);
void getNetAddress(int *msk1, int *msk2, int *msk3, int *msk4,int ip1, int ip2, int ip3, int ip4, char class);
void getBroadcastAddress(int *tmpmsk1, int *tmpmsk2, int *tmpmsk3, int *tmpmsk4,int ip1, int ip2, int ip3, int ip4,char class);
int getAvailableIP(char class);
void checkIP();

