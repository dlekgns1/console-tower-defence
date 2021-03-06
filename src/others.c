#include <stdio.h>
#include <ctype.h>

#include "console_draw2.h"
#include "TD.h"
#include "kb_input2.h"

extern void writer(void);
extern void intro(void);
extern void rankList(void);
extern void saveScore(int FinScore);

void writer(void)
{
	FILE *in;
	char ch;
	int i,j;
	int vk;
	
	clearScreen();
	
	in = fopen("Writer.txt","r");
	/* 把檔案�堶悸漲r讀取並顯示出來 */
	for(i=0;i<HEIGHT;i++) {
		for(j=0;j<WIDTH*2;j++) {
			while((ch = fgetc(in)) == '\n' || ch == '|' || ch == '-');  
			putASCII2(j + OFFSET_X,i + OFFSET_Y,ch,15,0);
		}
	}
	fclose(in);
	drawCmdWindow();
	
	/* 當使用者按下 Enter 鍵即離開 */
	while(1) {
		if(waitForKeyDown(1)) {
			vk=getKeyEventVirtual();
			if(vk == VK_RETURN)
				break;
		}
		if(i==0)
			putString(26+OFFSET_X,18+OFFSET_Y,"                    ",12,0);
		else
			putString(26+OFFSET_X,18+OFFSET_Y,"想離開請按 Enter ...",12,0);
		drawCmdWindow();
		i= !i;
	}
}

void intro(void) {
	FILE *in;
	char ch;
	int i,j;
	int vk;
	
	clearScreen();
	
	in = fopen("intro.txt","r");
	/* 把檔案�堶悸漲r讀取並顯示出來 */
	for(i=0;i<HEIGHT;i++) {
		for(j=0;j<WIDTH*2;j++) {
			while((ch = fgetc(in)) == '\n' || ch == '|' || ch == '-');  
			putASCII2(j + OFFSET_X,i + OFFSET_Y,ch,15,0);
		}
	}
	fclose(in);
	drawCmdWindow();
	
	/* 當使用者按下 Enter 鍵即離開 */
	while(1) {
		if(waitForKeyDown(1)) {
			vk=getKeyEventVirtual();
			if(vk == VK_RETURN)
				break;
		}
		if(i==0)
			putString(46+OFFSET_X,18+OFFSET_Y,"                    ",12,0);
		else
			putString(46+OFFSET_X,18+OFFSET_Y,"想離開請按 Enter ...",12,0);
		drawCmdWindow();
		i= !i;
	}
}

void rankList(void)
{
	FILE *in;
	char name[100];
	char toString[100];
	int score;
	int i=1;
	int vk;
	
	clearScreen();
	
	in = fopen("ranklist.slmt","r");
	
	putString(24+OFFSET_X,1+OFFSET_Y,"排     行     榜",11,5);
	
	while(fscanf(in,"%d,%s",&score,name) == 2) {
		
		sprintf(toString,"%d.  %7d",i,score);
		putString(14+OFFSET_X,i*2+1+OFFSET_Y,toString,15,0);
		putString(44+OFFSET_X,i*2+1+OFFSET_Y,name,15,0);
		
		i++;
	}
	
	fclose(in);
	drawCmdWindow();
	
	/* 當使用者按下 Enter 鍵即離開 */
	while(1) {
		if(waitForKeyDown(1)) {
			vk=getKeyEventVirtual();
			if(vk == VK_RETURN)
				break;
		}
		if(i==0)
			putString(46+OFFSET_X,18+OFFSET_Y,"                    ",12,0);
		else
			putString(46+OFFSET_X,18+OFFSET_Y,"想離開請按 Enter ...",12,0);
		drawCmdWindow();
		i= !i;
	}
}

void saveScore(int FinScore)
{
	FILE *in,*out;
	int i,j,temp;
	int k,l;
	char name[8][100];
	char input[100]="\0";
	char vk;
	char toString[10];
	int score[8];
	
	in = fopen("ranklist.slmt","r");
	
	for(i=0;i<8;i++) {
		fscanf(in,"%d,%s",&score[i],name[i]);
	}
	
	fclose(in);
	i=0;
	
	while(FinScore < score[i])
		i++;
		
	if( i < 8 ) {
		/* 把分數往後推一格 */
		for(j=7;j>i;j--)
			score[j] = score[j-1];
		/* 把姓名往後推一格 */
		for(j=7;j>i;j--)
			strcpy(name[j],name[j-1]);
		
		score[i]=FinScore;
		
		/* 要求輸入姓名 */
		j=0;
		while(1) {
			for(k=0;k<7;k++)
				for(l=0;l<32;l++)
					putASCII2(20 + l + OFFSET_X,8 + k + OFFSET_Y,' ',14,1);
			if(waitForKeyDown(0.5)) {
      		vk=getKeyEventASCII();
      		if((isalpha(vk) || isdigit(vk) || vk == '.') && j<=15) { /* 接受英文字母、數字和點 */
	      		input[j]=vk;
   	   		j++;
   	   		input[j]='\0';
				} else if (vk == 0x08 && j>0) { /* 倒退鍵  */
					j--;
					input[j]='\0';
				} else if (vk == 0x0D) { /* Enter 鍵  */
				   if(input[0] != '\0') {
   					strcpy(name[i],input);
					   break;
               }
				}
			}
      	
			sprintf(toString,"Yor Score:%5d",FinScore);
			putString(26+OFFSET_X,9+OFFSET_Y,toString,15,1);
			putString(23+OFFSET_X,10+OFFSET_Y,"Your Name: ",15,1);
			putString(34+OFFSET_X,10+OFFSET_Y,input,15,1);
			putString(22+OFFSET_X,12+OFFSET_Y,"請輸入您的名字(less than 15)",15,1);
			putString(28+OFFSET_X,13+OFFSET_Y,"按 Enter 完成",15,1);
			
			drawCmdWindow();
		}
		
	}
	
	
	out = fopen("ranklist.slmt","w");
	for(i=0;i<8;i++) {
		fprintf(out,"%d,%s\n",score[i],name[i]);
	}
	fclose(out);
}
