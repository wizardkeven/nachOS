#ifdef CHANGED

typedef struct{
	int f;
	int arg;
}argThreadUser;

extern int do_UserThreadCreate(int f, int arg);
extern void do_UserThreadExit();
extern void do_UserThreadJoin(int i);


#endif //CHANGED