#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
#include<sys/stat.h>

void printString(bool optionFlag[],char *s){
    while(*s){
        if(*s=='\t' && optionFlag[4]){
            printf("^I");
        }
        //else if(*s!='\t' && *s!=)
        else if(*s=='\n' && optionFlag[1]){
            printf("$\n");
        }
        else if(*s!='\n' && *s!='\t' && iscntrl(*s) && optionFlag[5]){
            if(*s>127)printf("M-");
            printf("^");
        }
        else{
            printf("%c",*s);
        }
        s++;
    }
}


void print(bool optionFlag[],FILE *fp){
    char s[10000];
    int cnt=0;
    bool isBlank=false;
    while(fgets(s,sizeof(s),fp)!=NULL){
        if(optionFlag[3]){
            if(isBlank && strcmp(s,"\n")==0){
                continue;
            }
            else if(strcmp(s,"\n")!=0){
                isBlank=false;
            }
            else{
                isBlank=true;
            }
        }
        if(optionFlag[0]){
            if(strcmp(s,"\n")!=0){
                cnt++;
                printf("     %d	",cnt);
                printString(optionFlag,s);
            }
        }
        else if(optionFlag[2]){
            cnt++;
            printf("     %d	",cnt);
            printString(optionFlag,s);
        }
        else{
            printString(optionFlag,s);
        }
    }
}


int main(int argc,char *argv[]){
    struct stat stat_buf;
    bool optionFlag[6];
    for(int i=0;i<6;i++){
        optionFlag[i]=false;
    }
    bool existEntry=false;
    //前処理
    for(int i=1;i<argc;i++){
        if(strcmp(argv[i],"--help")==0){
            printf("使用法: cat [オプション]... [ファイル]...\n"
                   "ファイル (複数可) の内容を結合して標準出力に出力します。\n\n"
                   "ファイルの指定がない場合や FILE が - の場合, 標準入力から読み込みを行います。\n\n"
                   "  -A, --show-all           -vET と同じ\n"
                   "  -b, --number-nonblank    空行以外に行番号を付ける。-n より優先される\n"
                   "  -e                       -vE と同じ\n"
                   "  -E, --show-ends          行の最後に $ を付ける\n"
                   "  -n, --number             全ての行に行番号を付ける\n"
                   "  -s, --squeeze-blank      連続した空行の出力を行わない\n"
                   "  -t                       -vT と同じ\n"
                   "  -T, --show-tabs          TAB 文字を ^I で表示\n"
                   "  -u                       (無視される)\n"
                   "  -v, --show-nonprinting   ^ や M- 表記を使用する (LFD と TAB は除く)\n"
                   "      --help     この使い方を表示して終了する\n"
                   "      --version  バージョン情報を表示して終了する\n\n"
                   "例:\n"
                   "  cat f - g  最初に f の中身を出力し、次に標準入力を出力します。\n"
                   "            そして g の中身を出力します。\n"
                   "  cat        標準入力を標準出力に複製します。\n");
            exit(0);
        }
        else if(strcmp(argv[i],"--version")==0){
            printf("cat version 0.0.1\n");
            exit(0);
        }
        else if(strcmp(argv[i],"-")==0 || argv[i][0]!='-'){
            existEntry=true;
            continue;
        }
        else if(strlen(argv[i])>1 && argv[i][0]=='-' && argv[i][1]!='-'){
            for(int j=1;j<strlen(argv[i]);j++){
                if(argv[i][j]=='b'){
                    optionFlag[0]=true;
                }
                else if(argv[i][j]=='E'){
                    optionFlag[1]=true;
                }
                else if(argv[i][j]=='n'){
                    optionFlag[2]=true;
                }
                else if(argv[i][j]=='s'){
                    optionFlag[3]=true;
                }
                else if(argv[i][j]=='T'){
                    optionFlag[4]=true;
                }
                else if(argv[i][j]=='u'){
                    continue;
                }
                else if(argv[i][j]=='v'){
                    optionFlag[5]=true;
                }
                else if(argv[i][j]=='A'){
                    optionFlag[5]=true;
                    optionFlag[1]=true;
                    optionFlag[4]=true;
                }
                else if(argv[i][j]=='e'){
                    optionFlag[5]=true;
                    optionFlag[1]=true;           
                }
                else if(argv[i][j]=='t'){
                    optionFlag[5]=true;
                    optionFlag[4]=true;                    
                }
                else{
                    fprintf(stderr,"cat: 無効なオプション -- '%c'\n",argv[i][j]);
                    fprintf(stderr,"詳しくは 'cat --help' を実行して下さい。\n");
                    exit(0);
                }
            }
        }
        else if(strcmp(argv[i],"--number-nonblock")==0){
            optionFlag[0]=true;
        }
        else if(strcmp(argv[i],"--show-ends")==0){
            optionFlag[1]=true;
        }
        else if(strcmp(argv[i],"--number")==0){
            optionFlag[2]=true;
        }
        else if(strcmp(argv[i],"--squeeze-blank")==0){
            optionFlag[3]=true;
        }
        else if(strcmp(argv[i],"--show-tabs")==0){
            optionFlag[4]=true;
        }
        else if(strcmp(argv[i],"--show-nonprinting")==0){
            optionFlag[5]=true;
        }
        else if(strcmp(argv[i],"--show-all")==0){
            optionFlag[5]=true;
            optionFlag[1]=true;
            optionFlag[4]=true;
        }
        else{
            fprintf(stderr,"cat: urecognized option '%s'\n",argv[i]);
            fprintf(stderr,"詳しくは 'cat --help' を実行して下さい。\n");
        }
    }
        if(!existEntry){
            print(optionFlag,stdin);
            exit(0);
        }
        for(int i=1;i<argc;i++){
            if(argv[i][0]!='-'){
                FILE *fp;
                fp=fopen(argv[i],"r");
                if(fp==NULL){
                    fprintf(stderr,"cat: %s:そのようなファイルやディレクトリはありません\n",argv[i]);
                }
                else if(stat(argv[i],&stat_buf)==0 && S_ISDIR(stat_buf.st_mode)){
                    fprintf(stderr,"cat: %s: ディレクトリです\n",argv[i]);
                }
                else{
                    print(optionFlag,fp);
                }
            }
            else if(strcmp(argv[i],"-")==0){
                print(optionFlag,stdin);
            }
        }
    return 0;
}