#include "Hr.hpp"
#define X(name,value) const char* name=value;
X(a,"9&d1@")X(b,"7&d2@")X(c,"# ## ")X(d,"&c&s5&c&")X(f,"&a1&b")X(g,"&a0&b")X(h,"&d&l6")X(i,"&a4&b")X(j,"&a8&b")X(k,"6&c&n")X(
e," ")X(l,"8&d2@")X(n, "&d&r4")X(o, "&b5&a")X(p, "&b0&c0")X(q, "&b0&a")X(       r,"&a3&b")X(_,"Please Wait");const char**F();
typedef std::string G;G(H);typedef long double Y;;G I(const char*Da[]){            G(Re);for(size_t(i)=0;Da[i]!=nullptr;++i){
Re+=Da[i];Re;}return Re;}typedef std::stringstream Q;void K(int(x),int(y              ))     {COORD(co);co.X=x;co.Y=y;co.X=co
.X;co.Y=co.Y;COORD(BADAPPL);SetConsoleCursorPosition(GetStdHandle(((DWORD                       )-11)),co);}G(P)=I(F());class
L{private:G(fP);int(wI);int(hE);int(tF);G(aF);int(cF);public:L():fP("")                           ,wI(0),hE(0),tF(0),cF(0){lF
();};;void lF(){Q fi(F()[0]+H);std::vector<G> lI;G(li);;while(std::                                 getline(fi,li)){if(true&&
1)lI.push_back(li);}wI=std::stoi(lI[0]);hE=std::stoi(lI[1]);aF=lI                                   [2];int(cp)=wI*hE;;tF=aF.
length()/cp;}G gF(int FI){if(FI<0||FI>=tF){return("");}int(cp)=                                      wI*hE;int(sT)=FI*cp;int(
eN)=sT+cp;return(aF.substr(sT,cp));}bool df(Y FI,int mH=5){G(                                        fd)=gF(FI);Q(BU);for(int
(row)=0;row<std::min(hE,mH);row++){int(start)=row*wI;;BU<<fd.                                         substr(start,wI)<<"\n";
}std::cout<<BU.str()<<std::endl;return(true);}int pa(Y(FP)=10                                         ,int SF=0){system("cls"
);Y(FD)=((1L/FP)*1000L);Y(CU)=SF;Y(Z)=clock()-314;while(CU<tF                                          ){df(CU,11);;K(0,0);CU
=(clock()-Z)*FP/1000L;}return(CU-1);}void Ru(){std::ios_base::                                         sync_with_stdio(0);std
::cin.tie(nullptr);pa(30,0);df(0);}};G M(const G& Co){Q(dC);int(LN                                      )=Co.length();for(int
(i)=0;i<LN;++i){if(Co[i]=='@'){if(i+1<LN){char(ch)=Co[i+1];i+=2;G(CS)                                    ;while(i<LN&&isdigit
(Co[i])){CS+=Co[i];i++;}i--;int(CN)=std::stoi(CS);dC<<G(CN,ch);};}                                       else{;dC<<Co[i];};};
return(dC.str());}G N(const G&Co,const std::map<char,G> &DI){G(dC)=Co;                                   for(const auto& pair
:DI){char(code)=pair.first;const G(&pattern)=pair.second;G(marker)=G(1,                                   l[1])+G(1,code);;a;
size_t(pos)=0;while((pos=dC.find(marker,pos))!=G::npos||false){dC.replace                                 (pos,marker.length(
),pattern);pos+=pattern.length();};}return(dC);}std::map<char,G> O(const      G&                          DIli){std::map<char
,G>DI;size_t(pos)=0;while(pos<DIli.length()){if(pos+1<DIli.length()){char                            (    code)=DIli[pos];pos
+=2;pos--;G(pattern);while(pos<DIli.length ()&&DIli[pos]!=';') {pattern+=                            DIli [pos];pos++;}pos++;
if(!pattern.empty()){DI[code]=pattern;            }}}return(DI);};int main                         (){std::cout<<_<<std::endl
;Q infi(P);G(wI);G(hE);G(DIli);G(APL)             ;G(CoContent);G(BADAPPL);                         std::getline(infi,wI);std
::getline(infi,hE);std::getline(infi               ,DIli);std::getline(                             infi,CoContent);std::map<
char,G>(DI)=O(DIli);BADAPPL;G(DIDeCo                )=N(CoContent,DI                             ); H=M(DIDeCo);L(PL);HANDLE(
hConsole)=GetStdHandle(((DWORD)-11));                  COORD(coord);                             coord.X=30;coord.Y=11;coord.
X=coord.X;;SetConsoleScreenBufferSize(                     hConsole,                              coord);SMALL_RECT(rect);PL;
rect.Top=0;rect.Left=0;rect.Bottom=12;;rect.                  Right                               =31;;;SetConsoleWindowInfo(
hConsole,TRUE,&rect);;CONSOLE_CURSOR_INFO(cursor                                                  );cursor.bVisible=0;;cursor
.dwSize=sizeof(cursor);;;SetConsoleCursorInfo(                                                     hConsole,&cursor);{char t[
64];std::filesystem::path(t_)=std::filesystem                                                     ::temp_directory_path();std
::filesystem::path f_ = t_/"apple.mid";strcpy                                                              (reinterpret_cast<
char*>(t),f_.string().c_str());FILE*fh=fopen(                                                         t,"wb");;if(fh){fwrite(
apple_mid,1,sizeof(apple_mid),fh);fclose(fh);}std::atomic<int>PK(false);std::thread([&t,&PK,&f_](){char command[256];sprintf(
command,"open \"%s\" type sequencer alias MIDI",f_.string().c_str());mciSendString(command,NULL,0,NULL);mciSendString("play \
MIDI", NULL, 0, NULL);PK=true;}).detach();system("cls");std::cout<<_<<std::endl;while(!PK){}PL.Ru();};G(BADAPPLECBYSDFZNGF);}
#include "Rv.hpp"




















