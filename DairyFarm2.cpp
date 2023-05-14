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
	virtual string info() = 0;   
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
    string info() { 
      return "cow"; 
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
   string info() { 
      return "goat"; 
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
   string info() { 
      return "sheep"; 
   }
};



enum Animal_id { Cow_id = 0, Goat_id, Sheep_id };

//будет синглтон
class Creator {
public:
	//virtual Animal* CreateAnimal (Animal_id);
	static Animal* CreateAnimal (Animal_id);
	static void DelAnimal(Animal* p){
		delete(p);
		return;
	}
};

Animal* Creator::CreateAnimal (Animal_id id) {
	if (id == Cow_id) return new Cow;
	if (id == Goat_id) return new Goat;
	if (id == Sheep_id) return new Sheep;
	//if (id == Con_id) return new Container;
	return 0;
}




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
	
	list<Animal*> listAnimal;
	//list<Cow> listCow;
	//list<Goat> listGoat;
	//list<Sheep> listSheep;
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
				
				Animal* a = Creator::CreateAnimal (Cow_id);
				listAnimal.push_back(a);
				a->gender();
				cout<<"you have one more cow"<<endl;
				
			} else if (strcmp(str, "goat") == 0){
				
				Animal* a = Creator::CreateAnimal (Goat_id);
				listAnimal.push_back(a);
				a->gender();
				cout<<"you have one more goat"<<endl;
				
			} else if (strcmp(str, "sheep") == 0){
				
				Animal* a = Creator::CreateAnimal (Sheep_id);
				listAnimal.push_back(a);
				a->gender();
				cout<<"you have one more sheep"<<endl;
				
			}
            break;
            
         case 2:// продать
			cout << "What animal do you want to sell?" << endl;
            cin>>str;
            
            
			if (strcmp(str, "cow") == 0){
				if (listAnimal.empty())
					cout << "Have no animal" << endl;
				else {
					int fl = 0;
					auto it = listAnimal.begin();
					while(it != listAnimal.end()){
						if((*it)->info() == "cow"){
							fl = 1;
							Creator::DelAnimal(*it);
							it = listAnimal.erase(it);
							cout << "Cow sold" << endl;
							break;
						}else{
							it++;
						}
					}
					if (fl == 0){
						cout << "Have no cow" << endl;
					}
				}
				
			} else if (strcmp(str, "goat") == 0){
				if (listAnimal.empty())
					cout << "Have no animal" << endl;
				else {
					int fl = 0;
					auto it = listAnimal.begin();
					while(it != listAnimal.end()){
						if( (*it)->info() == "goat" ){
							fl = 1;
							Creator::DelAnimal(*it);
							it = listAnimal.erase(it);
							cout << "Goat sold" << endl;
							break;
						}else{
							it++;
						}
					}
					if (fl == 0){
						cout << "Have no goat" << endl;
					}
				}
				
			} else if (strcmp(str, "sheep") == 0){
				if (listAnimal.empty())
					cout << "Have no animal" << endl;
				else {
					int fl = 0;
					auto it = listAnimal.begin();
					while(it != listAnimal.end()){
						if( (*it)->info() == "sheep" ){
							fl = 1;
							Creator::DelAnimal(*it);
							it = listAnimal.erase(it);
							cout << "Sheep sold" << endl;
							break;
						}else{
							it++;
						}
					}
					if (fl == 0){
						cout << "Have no sheep" << endl;
					}
				}
			}
         break;
            
         case 3://voice
			for (Animal* n : listAnimal){
				n->makesound();
			}
            break;
            
         case 4://покормить
			for (Animal* n : listAnimal){
				n->graze();
			}
            break;
            
         case 5://подоить
			for (Animal* n : listAnimal){
				if(n->gender_is()){
					n->getmilk();
					Container c;
					c.changev();
					//c.kind_is("cow");
					listCon.push_back(c);
				}
			}
			if (listAnimal.empty()){
				cout<< "Have no animal"<<endl;
			} 
            break;
            
         case 6:{
			cout<<"There are " <<listAnimal.size()<< " animals on the farm"<< endl;
			
			int a = 0,b = 0,c = 0;
			auto it = listAnimal.begin();
			while(it != listAnimal.end()){
				
				if ((*it)->info() == "cow"){
					a++;
					//cout<<"1cow"<<endl;
				}
				if ((*it)->info() == "goat"){
					b++;
					//cout<<"1goat"<<endl;
				}
				if ((*it)->info() == "sheep"){
					c++;
					//cout<<"1sheep"<<endl;
				}
				it++;
			}
			
            cout<<"There are " <<a<< " cows on the farm"<< endl;
            cout<<"There are " <<b<< " goats on the farm"<< endl;
            cout<<"There are " <<c<< " sheep on the farm"<< endl;
            break;
		}
            
         case 7:
			cout << "What do you want to sell?" << endl;
			cout<<" 1 - milk\n 2 - kefir\n 3 - cheese"<<endl;
            cin>>j;
			if (j == 1){//milk
				if (!listCon.empty()){
					Container c = listCon.back();
					listCon.pop_back();
					cout<<"We sell milk, volume is "<<c.look()<<endl;
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
            
         case 1000: {
			auto it = listAnimal.begin();
			while(it != listAnimal.end()){
				Creator::DelAnimal(*it);
				cout<<"minus one"<<endl;
				it++;
			}
			
			listAnimal.clear();
			//listCow.clear();
			//listGoat.clear();
			//listSheep.clear();
			listCon.clear();
			cout<<"end of prog"<<endl;
            exit(0);
            //break;
		}
            
         default:
           cout<<" no such command "<<endl;
           break; 
      }
	};
}
