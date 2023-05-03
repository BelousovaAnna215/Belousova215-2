#include <iostream>
#include <list>
#include <string.h>

using namespace std;

class Animal 
{ 
   string name;
   char animal_gender;
public:
	Animal(){
		//cout<<"animal constructor"<<endl;
	}
	void gender(){
		cout<<"What gender? (f/m)"<<endl;
		cin>>animal_gender;
	}
	bool gender_is(){
		cout<<animal_gender<<endl;
		 if (animal_gender == 'f')
			return true;
		else return false;
	}
    virtual void makesound(){
        cout<<"nothing"<<endl;
    }
    virtual void graze(){
        cout<<"who?"<<endl;
    } 
    virtual void getmilk(){
        cout<<"from who?"<<endl;
    }
    virtual ~Animal(){
		//cout<<"animal destructor"<<endl;
	}
};


class Cow : public Animal
{ 
public:
	Cow() {
        //cout<<"you have one more cow"<<endl;
    }
    void makesound() {
        cout<<"mooooo"<<endl;
    }
    virtual void graze(){
       cout<<"cow eat"<<endl;
	} 
	virtual void getmilk(){
		cout<<"have cow milk"<<endl;
	}
	~Cow() {
        //cout<<"one less cow"<<endl;
    }
};

class Goat : public Animal
{ 
public:
    void makesound(){
        cout<<"baaaaa"<<endl;
    }
    Goat() {
        //cout<<"you have one more goat"<<endl;
    }
    ~Goat() {
        //cout<<"one less goat"<<endl;
    }
    virtual void graze(){
       cout<<"goat eat"<<endl;
    }
    virtual void getmilk(){
       cout<<"have goat milk"<<endl;
   } 
};

class Sheep : public Animal
{ 
public:
    void makesound(){
        cout<<"baaaaa"<<endl;
    }
    Sheep() {
        //cout<<"you have one more sheep"<<endl;
    }
    ~Sheep() {
        //cout<<"one less sheep"<<endl;
    }
    virtual void graze(){
       cout<<"sheep eat"<<endl;
    }
    virtual void getmilk(){
       cout<<"have sheep milk"<<endl;
   } 
};


class Container 
{ 
    int volume;
    int maxvolume = 100;
    bool full;
    string kind;
 public:
    Container(){
        //cout<<"new container"<<endl;
    }
    ~Container(){
        //cout<<"~container"<<endl;
    }
    void kind_is(string s){
	    kind = s;
    }
    string kind_of(){
		return kind;
	}
    void changev(){
		cout<<"what is valume?"<<endl;
	    cin>>volume;
    }
    void sell(){
	    int price;
	    cout<<"what is price?"<<endl;
	    cin>>price;
        cout<<"sold for "<<price<<endl;
    }
    void makefull(){
	    full = true;
	    volume = maxvolume;
        cout<<"make full"<<endl;
    }
    void makeempty(){
	    full = false;
	    volume = 0;
        cout<<"make empty"<<endl;
    }
    void isfull(){
	    if (volume == maxvolume)
			cout<<"full"<<endl;
		else cout<<"not full"<<endl;
    }
    int look(){
		return volume;
	}
	//из молока сделать кефир
	void mkkefir(){
		cout<<"made kefir from milk"<<endl;
	}
	// из молока сделать сыр
	void mkcheese(){
		cout<<"made cheese from milk"<<endl;
	}
};

void menu(){
	cout<<"\n__________________________________________"<<endl;
	cout<<" 1 - Купить животное "<<endl;
	cout<<" 2 - Продать животное "<<endl;
	cout<<" 3 - Звук "<<endl;
	cout<<" 4 - Покормить "<<endl;
	cout<<" 5 - Подоить "<<endl;
	cout<<" 6 - Посмотреть кто есть на ферме "<<endl;
	
	cout<<" 7 - Продать продукты "<<endl;
	cout<<" 9 - Посмотреть сколько молока "<<endl;
	
	cout<<" 0 - Пропустить "<<endl;
	cout<<" 1000 - Выход "<<endl;
	return;
}


int main(){
	
	list<Cow> listCow;
	list<Goat> listGoat;
	list<Sheep> listSheep;
	list<Container> listCon;
    int i, j;
    char str[10];
    
    while(1){
		
		menu();
		cin>>i;
		
		switch(i){
         case 1:// купить
			cout << "What animal do you want to buy?" << endl;
			cin>>str;
			if (strcmp(str, "cow") == 0){
				Cow a;
				cout<<"you have one more cow"<<endl;
				a.gender();
				listCow.push_back(a);
			} else if (strcmp(str, "goat") == 0){
				Goat a;
				cout<<"you have one more goat"<<endl;
				a.gender();
				listGoat.push_back(a);
			} else if (strcmp(str, "sheep") == 0){
				Sheep a;
				cout<<"you have one more sheep"<<endl;
				a.gender();
				listSheep.push_back(a);
			}
            break;
            
         case 2:// продать
			cout << "What animal do you want to sell?" << endl;
            cin>>str;
			if (strcmp(str, "cow") == 0){
				if (listCow.empty())
					cout << "Have no cow" << endl;
				else {
					cout << "Cow sold" << endl;
					listCow.pop_back();
				}
			} else if (strcmp(str, "goat") == 0){
				if (listGoat.empty())
					cout << "Have no goat" << endl;
				else {
					cout << "Goat sold" << endl;
					listGoat.pop_back();
				}
			} else if (strcmp(str, "sheep") == 0){
				if (listSheep.empty())
					cout << "Have no sheep" << endl;
				else {
					cout << "Sheep sold" << endl;
					listSheep.pop_back();
				}
			}
            break;
            
         case 3://voice
            for (Cow n : listCow){
				n.makesound();
			}
			for (Goat n : listGoat){
				n.makesound();
			}
			for (Sheep n : listSheep){
				n.makesound();
			}
            break;
            
         case 4://покормить
            for (Cow n : listCow){
				n.graze();
			}
			for (Goat n : listGoat){
				n.graze();
			}
			for (Sheep n : listSheep){
				n.graze();
			}
            break;
            
         case 5://подоить
            for (Cow n : listCow){
				if(n.gender_is()){
					n.getmilk();
					Container c;
					c.changev();
					c.kind_is("cow");
					listCon.push_back(c);
				}
			}
			for (Goat n : listGoat){
				if(n.gender_is()){
					n.getmilk();
					Container c;
					c.changev();
					c.kind_is("goat");
					listCon.push_back(c);
				}
			}
			for (Sheep n : listSheep){
				if(n.gender_is()){
					n.getmilk();
					Container c;
					c.changev();
					c.kind_is("sheep");
					listCon.push_back(c);
				}
			}
			if (listSheep.empty() and listGoat.empty() and listCow.empty() ){
				cout<< "Have no animal"<<endl;
			}
            break;
            
         case 6:
            cout<<"There are " <<listCow.size()<< " cows on the farm"<< endl;
            cout<<"There are " <<listGoat.size()<< " goats on the farm"<< endl;
            cout<<"There are " <<listSheep.size()<< " sheep on the farm"<< endl;
            break;
            
         case 7:
			cout << "What do you want to sell?" << endl;
			cout<<" 1 - milk\n 2 - kefir\n 3 - cheese"<<endl;
            cin>>j;
			if (j == 1){//milk
				if (!listCon.empty()){
					Container c = listCon.back();
					listCon.pop_back();
					cout<<"We sell "<<c.kind_of()<<" milk, volume is "<<c.look()<<endl;
					c.sell();
				} else 
					cout<< "We haven't got milk"<<endl;
				
			} else if (j == 2){// kefir
				if (!listCon.empty()){
					Container c = listCon.back();
					listCon.pop_back();
					c.mkkefir();
					c.sell();
				} else 
					cout<< "We haven't got milk to make kefir"<<endl;
				
				
			} else if (j == 3){//cheese
				if (!listCon.empty()){
					Container c = listCon.back();
					listCon.pop_back();
					c.mkcheese();
					c.sell();
				} else 
					cout<< "We haven't got milk to make cheese"<<endl;
			}
            break;
            
            
         case 9:
            cout<<"There are " <<listCon.size()<< " conteiners on milk"<< endl;
            break;
            
         case 0:
            break;
            
         case 1000:
            exit(0);
            
         default:
           cout<<" no such command "<<endl;
           break; 
      }
	};
}

