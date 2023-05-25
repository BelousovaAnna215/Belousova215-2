#include <iostream>
#include <string>
#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

enum type_of_lex {// набор именованных констант
    LEX_NULL,                                                                                   
    LEX_AND/*and*/, LEX_BEGIN/*{*/, LEX_BOOL/*bool*/, LEX_CONSTS/*strconst*/, LEX_ELSE/*else*/, LEX_END/*}*/, LEX_IF/*if*/, LEX_FALSE/*false*/, LEX_INT/*int*/, /*9*/       
    LEX_NOT/*not*/, LEX_OR/*or*/, LEX_PROGRAM/*program*/, LEX_READ/*read*/, LEX_STRING/*string*/, LEX_TRUE/*true*/, LEX_WHILE/*while*/, LEX_WRITE/*write*/, /*17*/
    LEX_FIN/*@*/,                                                                                 
    LEX_SEMICOLON/*;*/, LEX_COMMA/*,*/, LEX_COLON/*:*/, LEX_ASSIGN/*=*/, LEX_LPAREN/*(*/, LEX_RPAREN/*)*/, LEX_EQ/*==*/, LEX_LSS/*<*/, /*26*/
    LEX_GTR/*>*/, LEX_PLUS/*+*/, LEX_MINUS/*-*/, LEX_TIMES/* * */, LEX_SLASH/*/*/, LEX_LEQ/*<=*/, LEX_NEQ/* != */, LEX_GEQ/*>=*/,              
    LEX_NUM/*число*/,                                                                                   
    LEX_ID/*идентефикатор*/,                                                                                 

    POLIZ_LABEL,                                                                             
    POLIZ_ADDRESS,                                                                             
    POLIZ_GO,                                                                                   
    POLIZ_FGO,                                                                               
    
    LEX_SWRITE, LEX_SASSIGN, LEX_SEQ, LEX_SLSS, LEX_SGTR, LEX_SPLUS, LEX_SNEQ, LEX_SID
};

/////////////////////////  Класс Lex  //////////////////////////

struct int_string{
    int           v_lex; //число
    string		  v_slex = ""; //строка
    bool is_int = true;
};

class Lex {
    type_of_lex   t_lex; 
    int           v_lex; //число
    string		  v_slex; //строка

public:
    Lex ( type_of_lex t = LEX_NULL, int v = 0, string u = ""): t_lex (t), v_lex (v), v_slex(u)  { }
    type_of_lex  get_type () const {
    	  return t_lex;
    }
    int get_value () const {
    	  return v_lex;
    }
    string get_svalue () const {
    	  return v_slex;
    }
    int_string get_int_string () const {
    	  return {v_lex,v_slex};
    }
    
  friend ostream & operator<< ( ostream &s, Lex l ); //выводит лексему на экран
};

//////////////////////  TID  ///////////////////////

class Ident;
vector<Ident> TID; //таблица идентефикаторов

 /////////////////////  Класс Ident  ////////////////////////////
 

class Ident {// для строки в таблице TID
    string      name; 
    bool        declare;
    type_of_lex type;
    bool        assign;
    int          value = 0;
    string 		svalue;

public:
    Ident() { 
        declare = false;
        assign  = false;
    }
    bool operator== ( const string& s ) const {
        return name == s;
    }
    Ident ( const string n ) {
        name    = n;
        declare = false;
        assign  = false;
    }
    string get_name () const { 
      return name;
    }
    bool get_declare () const { 
      return declare;
    }
    void put_declare () {
      declare = true;
    }
    type_of_lex get_type () const {
      return type;
    }
    void put_type ( type_of_lex t ) {
      type = t;
    }
    bool get_assign () const {
      return assign;
    }
    void put_assign () {
      assign = true;
    }
    int  get_value () const {
      return value;
    }
    void put_value ( int v ) {
      value = v;
    }
    
    string  get_svalue () const {
      return svalue;
    }
    void put_svalue ( string v ) {
      svalue = v;
    }
    
	static void print_TID(){// вывод таблицы
		cout<<"TID"<<endl;
		 for(auto& ident : TID){
			 cout<<ident.name<<" "<<ident.declare<<" "<<ident.type<<" "<<ident.assign<<" "<<ident.value<<" "<<ident.svalue<<endl;
		 }
		 cout<<endl;
	 }
};


int put ( const string & buf ) {// добавить в TID в одном экземпляре
    vector<Ident>::iterator k; 

    if ( ( k = find ( TID.begin (), TID.end (), buf ) ) != TID.end () )
        return k - TID.begin();
    TID.push_back ( Ident(buf) );
    return TID.size () - 1; 
}


//////////////////////////////// Класс Scanner /////////////////////////////////

class Scanner {
    FILE * fp;
    char   c;
    int look ( const string buf, const char ** list ) {// позиция элемента в массиве
        int i = 0;
        while ( list[i] ) {
            if ( buf == list[i] )
                return i;
            ++i;
        }
        return 0;
    }
    void gc () {
        c = fgetc (fp);
    }

public:
    static const char * TW [], * TD [];

    Scanner ( const char * program ) {
        if ( !(fp = fopen ( program, "r" )) )
            throw  "can’t open file" ;
    }

    Lex get_lex (); // функция получение лексемы
};


const char *
Scanner::TW    [] = { "", "and", "{", "bool", "conststr", "else", "}", "if", "false", "int", "not", "or", "program",
                      "read", "string", "true", "while", "write", NULL };

const char *
Scanner::TD    [] = { "@", ";", ",", ":", "=", "(", ")", "==", "<", ">", "+", "-", "*", "/", "<=", "!=", ">=", NULL };

Lex Scanner::get_lex () {
    enum    state { H, IDENT, NUMB, COM, ALE, NEQ, STRG };// начало символ цифра (/*) (= < >) (!) строка
    int     d, j;
    string strg; // полученная строка
    string  buf; // полученная лексема
    state   CS = H; // текущее положение
    do {
        gc ();
        switch ( CS ) {
            case H:
                if ( c==' ' || c == '\n' || c== '\r' || c == '\t' );
                
                    else if ( isalpha (c) ) {
                        buf.push_back (c);
                        CS  = IDENT;
                    }
                    
                    else if ( c == '"' ) {
						//strg.push_back (c); 
                        CS  = STRG;
                    }
                    
                    else if ( isdigit (c) ) {
                        d   = c - '0';
                        CS  = NUMB;
                    }
                    
                    else if ( c == '/' ) { //комментарий
						gc();
						if( c == '*'){
							CS  = COM;
						} else {
							ungetc ( c, fp );
							if ( ( j = look ( "/", TD) ) ){
								return Lex ( (type_of_lex)( j + (int) LEX_FIN ), j );
							}
						}
                    }
                    
                    else if ( c == '{' ) { 
						j = look ( buf, TW);
                        return Lex ( LEX_BEGIN, j );
                    }
                    else if ( c == '}' ) { 
						j = look ( buf, TW);
                        return Lex ( LEX_END, j );
                    }
                    else if ( c == '=' || c == '<' || c == '>' ) {
                        buf.push_back (c);
                        CS  = ALE;
                    }
                    else if (c == '@'){
						j = look ( buf, TD);
                        return Lex ( LEX_FIN, j );
					}
                    else if (c == '!') {
                        buf.push_back (c);
                        CS  = NEQ;
                    }
                    else {
                        buf.push_back (c);
                        if ( ( j = look ( buf, TD) ) ){
                            return Lex ( (type_of_lex)( j + (int) LEX_FIN ), j );
                        }
                        else
                            throw c;
                    }
                    break;

            case IDENT:// символ
                if ( isalpha (c) || isdigit (c) || c == '"' ) {
                    buf.push_back (c);
                }
                else {
                    ungetc ( c, fp );
                    if ( (j = look ( buf, TW) ) ) {
                        return Lex ( (type_of_lex) j, j );
                        }
                        else {
                        j   = put ( buf );// добавляет в таблицу имя
                        return Lex ( LEX_ID, j );
                    }
                }
                break;

            case NUMB: // цифра
                if ( isdigit (c) ) {
                    d = d * 10 + ( c - '0' );
                }
                else {
                    ungetc ( c, fp );
                    return Lex ( LEX_NUM, d );
                }
                break;

            case COM:
				if ( c == '*' ) {
                    gc();
                    if ( c == '/' ){
						CS  = H;
					}
                }
                else if ( c == '@')
                    throw c;
                break;

            case ALE:
                if ( c == '=' ) {//<= >= ==
                    buf.push_back ( c );
                    j   = look ( buf, TD );
                    return Lex ( (type_of_lex) ( j + (int) LEX_FIN ), j );
                }
                else {// < > =
                    ungetc ( c, fp );
                    j   = look ( buf, TD );
                    return Lex ( (type_of_lex) ( j + (int) LEX_FIN ), j );
                }
                break;

            case NEQ:
                if ( c == '=' ) {// !=
                    buf.push_back(c);
                    j   = look ( buf, TD );
                    return Lex ( LEX_NEQ, j );
                }
                else
                    throw '!';
                break;
                
            case STRG:
                if ( c != '"'){
					strg.push_back(c);
				} else {
					//strg.push_back(c);
					return Lex ( LEX_CONSTS,0, strg );
				}
                break;
    } //end switch
  } while (true);
}


ostream & operator<< ( ostream &s, Lex l ) {
    string t;
    if ( l.t_lex <= LEX_WRITE )
        t = Scanner::TW[l.t_lex];
    else if ( l.t_lex >= LEX_FIN && l.t_lex <= LEX_GEQ )
        t = Scanner::TD[ l.t_lex - LEX_FIN ];
    else if ( l.t_lex == LEX_NUM )
        t = "NUMB";
    else if ( l.t_lex == LEX_ID )
        t = TID[l.v_lex].get_name ();
    else if ( l.t_lex == POLIZ_LABEL )
        t = "Label";
    else if ( l.t_lex == POLIZ_ADDRESS )
        t = "Addr";
    else if ( l.t_lex == POLIZ_GO )
        t = "!";
    else if ( l.t_lex == POLIZ_FGO )
        t = "!F";
             
    else if ( l.t_lex == LEX_SWRITE )
        t = "write_str";
    else if ( l.t_lex == LEX_SASSIGN )
        t = "=_str";
    else if ( l.t_lex == LEX_SEQ )
        t = "==_str";
    else if ( l.t_lex == LEX_SLSS )
        t = "<_str";
    else if ( l.t_lex == LEX_SGTR )
        t = ">_str";
    else if ( l.t_lex == LEX_SPLUS )
        t = "+_str";
    else if ( l.t_lex == LEX_SNEQ )
        t = "!_str";
    else if ( l.t_lex == LEX_SID )
        t = "id_str";
       
    else
        throw l;
    s << '(' << t << ',' << l.v_lex << ',' << l.v_slex << ");"  << endl;
    return s;
}


//////////////////////////  Класс Parser  /////////////////////////////////

template <class T, class T_EL>
void from_st ( T & st, T_EL & i ) {// забрать с верхушки стека 
    i = st.top(); st.pop();
}


// создаем вектор полиз
class Parser {

    Lex          curr_lex; 
    type_of_lex  c_type; 
    int          c_val;
    string 		 c_sval;
    
    bool flag = 0; // 0 - int 1 - string

    Scanner      scan; 

    stack < int >           st_int;
    stack < type_of_lex >   st_lex;

    void  P();
    void  D1();
    void  D(type_of_lex);
    void  B1();
    void  B();
    void  S(type_of_lex);
    void  C();
    void  T();

    void  W();
    void  W1();
    void  W2();
    void  W3();
    void  W4();
    void  W5();
    void  W6();

    void  dec ( type_of_lex type);
    void  check_id ();
    void  check_op ();
    void  check_not ();
    void  eq_type ();
    void  eq_bool ();
    void  check_id_in_read ();
    
    void  gl () {
        curr_lex  = scan.get_lex ();// получаем одну лексему
        //cout<<curr_lex;
        c_type    = curr_lex.get_type ();
        c_val     = curr_lex.get_value ();
        c_sval 	  = curr_lex.get_svalue();
    }

public:
    vector <Lex> poliz;
    Parser ( const char *program ) : scan (program) { }
    void  analyze();
    
};

void Parser::analyze () {
	cout<<"analyze"<<endl;
    gl ();
    P ();
    cout<<"\n\nПОЛИЗ\n";
    for ( Lex l : poliz )
        cout << l;
    cout << endl << "Yes!!!" << endl;
}

// P
//program { D1 B1 } @
void Parser::P () {
	//cout<<"P"<<endl;
    if ( c_type == LEX_PROGRAM ) {
        gl ();
    }
    else
        throw curr_lex;
    if ( c_type == LEX_BEGIN ) { //{
		gl();
		D1 ();
		B1 ();
	}
	else 
		throw curr_lex;
		
	if ( c_type != LEX_END ) { //} 
		throw curr_lex;
	}
	gl();
		
    if ( c_type != LEX_FIN ) { //@
		throw curr_lex;
	} else {
		cout<<"успешно"<<endl;
		Ident::print_TID();
	}
}

// D1
// D1  〈описания〉 → {{ 〈описание〉D;}}
void Parser::D1 () {
	while (c_type == LEX_INT or c_type == LEX_BOOL or c_type == LEX_STRING ){
		//cout<<"D1"<<endl;
		if (c_type == LEX_INT){
			D (LEX_INT);
		}
		if (c_type == LEX_BOOL){
			D (LEX_BOOL);
		}
		if (c_type == LEX_STRING){
			D (LEX_STRING);
		}
		gl();
	}
	//cout<<"описание закончилось"<<endl;
}

// D
// D  〈описание〉 → 〈тип〉T 〈переменная〉S {{ , 〈переменная〉S }}
void Parser::D (type_of_lex type_Lex) {
	//cout<<"D"<<endl;
	gl();
	if ( c_type != LEX_ID ) {
		throw curr_lex;
	}
	else {
		S(type_Lex);
		while ( c_type == LEX_COMMA ) {
            gl ();
            S(type_Lex);
        }
        if (c_type != LEX_SEMICOLON){ //;
			throw curr_lex;
		}
		dec(type_Lex); // записываем информацию в TID
	}
}


// S
// S  〈переменная〉 → 〈идентификатор〉ID | 〈идентификатор〉ID = 〈константа〉C
void Parser::S (type_of_lex type_Lex) {
	//cout<<"S"<<endl;
	st_int.push ( c_val );	
	gl();
    if ( c_type == LEX_ASSIGN ) {
		gl();
		C(); // занести значение в таблицу
	}
}

// C
// C  〈константа〉 → 〈целочисленная〉N | 〈строковая〉I
void Parser::C () {
	//cout<<"C"<<endl;
	if ( c_type == LEX_CONSTS ) {
        auto & a = TID.back();
		a.put_svalue(c_sval);
		a.put_assign();
		gl();
    }
    else 
    if ( c_type == LEX_NUM ) {
		auto & a = TID.back();
		a.put_value(c_val);
		a.put_assign();
		gl();
    }
}

// B1
// B1  〈операторы〉 → {{ 〈оператор〉B; }}
void Parser::B1 () {
	//cout<<"B1"<<endl;
	while ( c_type != LEX_END) {
		flag = 0;
		B ();
		gl();
	}
}

// B
// B  〈оператор〉 → if (〈выражение〉W) 〈оператор〉B else 〈оператор〉B | while (〈выражение〉W) 〈оператор〉B | read (〈идентификатор〉ID); |
//write (〈выражение〉W {{ , 〈выражение〉 W}} ); | 〈составной оператор〉
void Parser::B () {
	//cout<<"B"<<endl;

    int pl0, pl1, pl2, pl3;

    if ( c_type == LEX_IF ) {
		//cout<<"if"<<endl;
		gl ();
        W1 (); 
        eq_bool ();
        pl2 = poliz.size();
        poliz.push_back ( Lex() );
        poliz.push_back ( Lex(POLIZ_FGO) );
        
		B ();
		pl3 = poliz.size ();
		poliz.push_back ( Lex () );

		poliz.push_back ( Lex ( POLIZ_GO ) );
		poliz[pl2] = Lex ( POLIZ_LABEL, poliz.size() );
		gl();

		if ( c_type == LEX_ELSE ) {
			gl ();
			B ();
			poliz[pl3] = Lex ( POLIZ_LABEL, poliz.size() );
		}
		else
			throw curr_lex;
        
    }//end if


    else if ( c_type == LEX_WHILE ) {
		//cout<<"while"<<endl;
        pl0 = poliz.size ();
        gl ();
        W1 ();
        eq_bool ();
        pl1 = poliz.size ();
        poliz.push_back ( Lex () );
        poliz.push_back ( Lex (POLIZ_FGO) );
        
		B();
		poliz.push_back ( Lex ( POLIZ_LABEL, pl0 ) );
		poliz.push_back ( Lex ( POLIZ_GO) );
		poliz[pl1] = Lex ( POLIZ_LABEL, poliz.size() );
        
        }//end while


        else if ( c_type == LEX_READ ) {
			//cout<<"read"<<endl;
            gl ();
            if ( c_type == LEX_LPAREN ) {
                gl ();
                if ( c_type == LEX_ID ) {
                    check_id_in_read ();
                    poliz.push_back ( Lex( POLIZ_ADDRESS, c_val) );
                    gl();
                }
                else
                    throw curr_lex;
                if ( c_type == LEX_RPAREN ) {
                    gl ();
                    poliz.push_back ( Lex (LEX_READ) );
                }
                else
                    throw curr_lex;
            }
            else
                throw curr_lex;
        }//end read


        else if ( c_type == LEX_WRITE ) {
			//cout<<"write"<<endl;
            gl ();
            if ( c_type == LEX_LPAREN ) {
                gl ();
                W1 ();
                while ( c_type == LEX_COMMA){
					if (flag == 0){
						poliz.push_back ( Lex ( LEX_WRITE ) );
					}
					else
						poliz.push_back ( Lex ( LEX_SWRITE ) );
					gl ();
					W1 ();
				}
                if ( c_type == LEX_RPAREN ) {
                    gl ();
                    if (flag == 0){
						poliz.push_back ( Lex ( LEX_WRITE ) );
					}
					else
						poliz.push_back ( Lex ( LEX_SWRITE ) );
                }
                else
                    throw curr_lex;
            }
            else
                throw curr_lex;
        }//end write
        
        else if ( c_type == LEX_BEGIN ) {
			//cout<<"составной оператор"<<endl;
			gl();
			B1();
			if ( c_type != LEX_END ) {
				throw curr_lex;
			}
        }//составной оператор
		
		else{
			//cout<<"оператор выражение"<<endl;
            if ( c_type == LEX_ID ) {
				check_id ();
				poliz.push_back (Lex ( POLIZ_ADDRESS, c_val ) );
				gl();
				if ( c_type == LEX_ASSIGN ) {
					gl ();
					W1();
					eq_type ();
					if (flag == 0){
						poliz.push_back ( Lex ( LEX_ASSIGN ) );
					}
					else
						poliz.push_back ( Lex ( LEX_SASSIGN ) );
				}
				else
					throw curr_lex;
			} else {
				throw curr_lex;
			}
			if ( c_type != LEX_SEMICOLON ){
				throw curr_lex;
			}
		}
}

// W1 -> W2 {{[or] W2}}
void Parser::W1 () {
	//cout<<"W1"<<endl;
    W2 ();
    while (c_type == LEX_OR) {
        st_lex.push ( c_type );
        gl ();
        W2 ();
        check_op ();
    }

}

// W2 -> W3 {{[and] W3}}
void Parser::W2 () {
	//cout<<"W2"<<endl;
    W3 ();
    while (c_type == LEX_AND) {
        st_lex.push ( c_type );
        gl ();
        W3 ();
        check_op ();
    }

}

// W3 -> W4 [ == | < | > | <= | >= | != ] W4 |W4
void Parser::W3 () {
	//cout<<"W3"<<endl;
    W4 ();
    if ( c_type == LEX_EQ  || c_type == LEX_LSS || c_type == LEX_GTR ||
         c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ ) {
        st_lex.push ( c_type );
        gl ();
        W4 ();
        check_op ();
    }
}

// W4 -> W5 {{ [+|-]W5 }}
void Parser::W4 () {
	//cout<<"W4"<<endl;
    W5 ();
    while ( c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_SPLUS ) {
        st_lex.push ( c_type );
        gl ();
        W5 ();
        check_op ();
    }

}

// W5 -> W6 {{ [*|/]W6 }}
void Parser::W5 () {
	//cout<<"W5"<<endl;
    W6 ();
    while ( c_type == LEX_TIMES || c_type == LEX_SLASH ) {
        st_lex.push ( c_type );
        gl ();
        W6 ();
        check_op ();
    }

}

// W6 -> цифра|слово| not(W6) | (W)
void Parser::W6 () {
	//cout<<"W6"<<endl;
    if ( c_type == LEX_ID ) {
        check_id ();
        if ( flag == 0 ){
			poliz.push_back ( Lex ( LEX_ID, c_val ) );
		} else {
			poliz.push_back ( Lex ( LEX_SID, c_val ) );
		}
        gl ();
    }
    else if ( c_type == LEX_NUM ) {
        st_lex.push ( LEX_INT );
        poliz.push_back ( curr_lex );
        gl ();
        flag = 0;
    }
    else if ( c_type == LEX_CONSTS ) {
        st_lex.push ( LEX_STRING );
        poliz.push_back ( curr_lex );
        gl ();
        flag = 1;
    }
    else if ( c_type == LEX_TRUE ) {
        st_lex.push ( LEX_BOOL );
        poliz.push_back ( Lex (LEX_TRUE, 1) );
        gl ();
        flag = 0;
    }
    else if ( c_type == LEX_FALSE) {
        st_lex.push ( LEX_BOOL );
        poliz.push_back ( Lex (LEX_FALSE, 0) );
        gl ();
        flag = 0;
    }
    else if ( c_type == LEX_NOT ) {
        gl ();
        W6 ();
        check_not ();
    }
    else if ( c_type == LEX_LPAREN ) {
        gl ();
        W1 ();
        if ( c_type == LEX_RPAREN)
            gl ();
        else
            throw curr_lex;
    }
    else
        throw curr_lex;
}


////////////////////////////////////////////////////////////////


// контролирует повторное описание
void Parser::dec ( type_of_lex type ) {
	//cout<<"dec"<<endl;
    int i;
    while ( !st_int.empty () ) {
        from_st ( st_int, i );
        if ( TID[i].get_declare () )
            throw "twice";
        else {
            TID[i].put_declare ();
            TID[i].put_type ( type );
        }
    }
}

void Parser::check_id () {
    //cout<<"check_id"<<endl;
    if ( TID[c_val].get_type() == LEX_STRING){
		flag = 1;
	}
    if ( TID[c_val].get_declare() )
        st_lex.push ( TID[c_val].get_type () );
    else{
        throw "not declared";
	}      
}

void Parser::check_op () {
    //cout<<"check_op"<<endl;
    
    type_of_lex t1, t2, op, t = LEX_INT, r = LEX_BOOL;
 
    from_st ( st_lex, t2 );
    from_st ( st_lex, op );
    from_st ( st_lex, t1 );
    
    //cout<<t1<<" "<<op<<" "<<t2<<endl;
 
    if ( op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH )
        r = LEX_INT;
    if ( op == LEX_OR || op == LEX_AND ){
        t = LEX_BOOL;
        flag = 0;
	}
    if ( op == LEX_PLUS && t1 == LEX_STRING ){
		r = LEX_STRING;
		t = LEX_STRING;
	}
	if (t1 == LEX_STRING && ( op == LEX_EQ || op == LEX_LSS || op == LEX_GTR || op == LEX_NEQ )){
		r = LEX_BOOL;
		t = LEX_STRING;
	}
    if ( t1 == t2  &&  t1 == t ) 
        st_lex.push (r);
    else
        throw "wrong types are in operation";
    
    if (flag == 1) {
		if (op == LEX_PLUS){
			poliz.push_back (Lex (LEX_SPLUS) );
		}
		else if (op == LEX_EQ){
			poliz.push_back (Lex (LEX_SEQ) );
		}
		else if (op == LEX_LSS){
			poliz.push_back (Lex (LEX_SLSS) );
		}
		else if (op == LEX_GTR){
			poliz.push_back (Lex (LEX_SGTR) );
		}
		else if (op == LEX_NEQ){
			poliz.push_back (Lex (LEX_SNEQ) );
		}
	} else{
		poliz.push_back (Lex (op) );
	}
}

void Parser::check_not () {
    //cout<<"check_not"<<endl;
    if (st_lex.top() != LEX_BOOL)
        throw "wrong type is in not";
    else
        poliz.push_back ( Lex (LEX_NOT) );
}

void Parser::eq_type () {
    //cout<<"eq_type"<<endl;
    type_of_lex t;
    from_st ( st_lex, t );
    if ( t != st_lex.top () )
        throw "wrong types are in =";
    st_lex.pop();
}

void Parser::eq_bool () {
    //cout<<"eq_bool"<<endl;
    if ( st_lex.top () != LEX_BOOL )
        throw "expression is not boolean";
    st_lex.pop ();
  }

void Parser::check_id_in_read () {// проверка для read
    //cout<<"check_id_in_read"<<endl;
    if ( !TID [c_val].get_declare() )
        throw "not declared";
}


////////////////////////////////////////////////////////////////
 
class Executer { // исполнитель
	
public:
    void execute ( vector<Lex> & poliz );
};

struct st_arg{
	
	stack < int_string > args;
	void push(int iv){
		args.push({iv,"",true});
	}
	void push(string sv){
		args.push({0,sv,false});
	}
	int_string top_pop(){
		int_string v = args.top();
		args.pop();
		return v;
	}
	int top(){
		int_string v = args.top();
		return v.v_lex;
	}
	void pop(){
		args.pop();
	}
		
};
 
void Executer::execute ( vector<Lex> & poliz ) {
    Lex pc_el; 
    st_arg args;
    
    string u, v;
    int i, j, index = 0, size = poliz.size();
    while ( index < size ) {
        pc_el = poliz [ index ];
        switch ( pc_el.get_type () ) {
			
			
			case LEX_CONSTS:
				//cout<<"LEX_CONSTS ="<<pc_el.get_svalue ()<<endl;
				args.push ( pc_el.get_svalue () );
				break;
			
            case LEX_TRUE:
            case LEX_FALSE: 
            case LEX_NUM: 
            case POLIZ_ADDRESS: 
            case POLIZ_LABEL:
				//cout<<"LEX_..."<<endl;
                args.push ( pc_el.get_value () );
                break;
 
            case LEX_ID:
				//cout<<"LEX_ID"<<endl;
                i = pc_el.get_value ();
                if ( TID[i].get_assign () ) {
                  args.push ( TID[i].get_value () );
                  break;
                }
                else
                  throw "POLIZ: indefinite identifier";
                  
            case LEX_SID:
				//cout<<"LEX_SID"<<endl;
                i = pc_el.get_value ();
                if ( TID[i].get_assign () ) {
                  args.push ( TID[i].get_svalue () );
                  break;
                }
                else
                  throw "POLIZ: indefinite identifier";
 
            case LEX_NOT:
				//cout<<"LEX_NOT"<<endl;
                from_st ( args, i );
                args.push( !i );
                break;
 
            case LEX_OR:
				//cout<<"LEX_OR"<<endl;
                from_st ( args, i ); 
                from_st ( args, j );
                args.push ( j or i );
                break;
 
            case LEX_AND:
                //cout<<"LEX_AND"<<endl;
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j and i );
                break;
 
            case POLIZ_GO:
                //cout<<"POLIZ_GO"<<endl;
                from_st ( args, i );
                index = i - 1;
                break;
 
            case POLIZ_FGO:
                //cout<<"POLIZ_FGO"<<endl;
                from_st ( args, i );
                from_st ( args, j );
                if ( !j ) index = i - 1;
                break;
 
            case LEX_WRITE:
                //cout<<"LEX_WRITE"<<endl;
                from_st ( args, j );
                cout << j << endl;
                break;
            
            case LEX_SWRITE:
                //cout<<"LEX_SWRITE"<<endl;
                u = args.top_pop().v_slex;
                cout << u << endl;
                break;
 
            case LEX_READ:{
                //cout<<"LEX_READ"<<endl;
                int k;
                string u;
                from_st ( args, i );
                if ( TID[i].get_type () == LEX_INT ) {
                    cout << "Input int value for " << TID[i].get_name () << endl;
                    cin >> k;
                    TID[i].put_value (k);
					TID[i].put_assign ();
                } else if (TID[i].get_type () == LEX_STRING) {
					cout << "Input string for " << TID[i].get_name () << endl;
                    cin >> u;
                    TID[i].put_svalue (u);
					TID[i].put_assign ();
				}
                else {
                    string j;
                    while (1) {
                        cout << "Input boolean value (true or false) for " << TID[i].get_name() << endl;
                        cin >> j;
                        if ( j != "true" && j != "false" ) {
                            cout << "Error in input:true/false" << endl;
                            continue;
                        }
                        k = ( j == "true" ) ? 1 : 0;
                        break;
                    }
                    TID[i].put_value (k);
					TID[i].put_assign ();
                }
                break;
			}
                
 
            case LEX_PLUS:
                //cout<<"LEX_PLUS"<<endl;
                from_st ( args, i );
                from_st ( args, j );
                args.push ( i + j );
                break;
                
            case LEX_SPLUS:
                //cout<<"LEX_SPLUS"<<endl;
                u = args.top_pop().v_slex;
                v = args.top_pop().v_slex;
                args.push ( v + u );
                break;
 
            case LEX_TIMES:
                //cout<<"LEX_TIMES"<<endl;
                from_st ( args, i );
                from_st ( args, j );
                args.push ( i * j );
                break;
 
            case LEX_MINUS:
                //cout<<"LEX_MINUS"<<endl;
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j - i );
                break;
 
            case LEX_SLASH:
                //cout<<"LEX_SLASH"<<endl;
                from_st ( args, i );
                from_st ( args, j );
                if (i) {
                    args.push ( j / i );
                    break;
                }
                else
                    throw "POLIZ:divide by zero";
 
            case LEX_EQ:
                //cout<<"LEX_EQ"<<endl;
                from_st ( args, i );
                from_st ( args, j );
                args.push ( i == j );
                break;
                
            case LEX_SEQ:
                //cout<<"LEX_SEQ"<<endl;
                u = args.top_pop().v_slex;
                v = args.top_pop().v_slex;
                args.push ( v == u );
                break;
 
            case LEX_LSS:
                //cout<<"LEX_LSS"<<endl;
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j < i );
                break;
                
            case LEX_SLSS:
                //cout<<"LEX_SLSS"<<endl;
                u = args.top_pop().v_slex;
                v = args.top_pop().v_slex;
                args.push ( v < u );
                break;
 
            case LEX_GTR:
                //cout<<"LEX_GTR"<<endl;
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j > i );
                break;
               
            case LEX_SGTR:
                //cout<<"LEX_SGTR"<<endl;
                u = args.top_pop().v_slex;
                v = args.top_pop().v_slex;
                args.push ( v > u );
                break;
 
            case LEX_LEQ:
                //cout<<"LEX_LEQ"<<endl;
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j <= i );
                break;
 
            case LEX_GEQ:
                //cout<<"LEX_GEQ"<<endl;
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j >= i );
                break; 
 
            case LEX_NEQ:
                //cout<<"LEX_NEQ"<<endl;
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j != i );
                break;
                
            case LEX_SNEQ:
                //cout<<"LEX_SNEQ"<<endl;
                u = args.top_pop().v_slex;
                v = args.top_pop().v_slex;
                args.push ( v != u );
                break;
                
 
            case LEX_ASSIGN:
                //cout<<"LEX_ASSIGN"<<endl;
                from_st ( args, i );
                from_st ( args, j );
                TID[j].put_value (i);
                TID[j].put_assign (); 
                break;
                
            case LEX_SASSIGN:
                //cout<<"LEX_SASSIGN"<<endl;
                u = args.top_pop().v_slex;
                from_st ( args, j );
                TID[j].put_svalue (u);
                TID[j].put_assign (); 
                break;

            default:
				cout<<"default  ";
				cout<<pc_el.get_type ()<<endl;
                throw "POLIZ: unexpected elem";
        }//end of switch
        ++index;
    };//end of while
    cout << "Finish of executing!!!" << endl;
}


/////////////////////////////////////////

class Interpretator {
    Parser   pars;
    Executer E;
public:
    Interpretator ( const char* program ): pars (program) {}
    void     interpretation ();
};

void Interpretator::interpretation () {
    pars.analyze ();
    cout<< " analyze is ready"<<endl;
    cout<< "Result executing:"<<endl;
    E.execute ( pars.poliz );
}

int main () {
	cout<< " Start!"<<endl;
    try {
        Interpretator I ( "ex1.txt" );
        I.interpretation ();
        return 0;
    }
    catch ( char c ) {
        cout << "unexpected symbol " << c << endl;
        return 1;
    }
    catch ( Lex l ) {
        cout << "unexpected lexeme" << l << endl;
        return 1;
    }
    catch ( const char *source ) {
        cout << source << endl;
        return 1;
    }
}

