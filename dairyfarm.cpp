#include<iostream>
using namespace std;

class Animal 
{ 
   string name;
public:
	Animal(){
		cout<<"animal constructor"<<endl;
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
    ~Animal(){
		cout<<"animal destructor"<<endl;
	}
};

class Cow : public Animal
{ 
public:
	Cow() {
        cout<<"you have one more cow"<<endl;
    }
    virtual void makesound() {
        cout<<"mooooo"<<endl;
    }
    virtual void graze(){
       cout<<"cow eat"<<endl;
	} 
	virtual void getmilk(){
		cout<<"have cow milk"<<endl;
	}
	~Cow() {
        cout<<"one less cow"<<endl;
    }
};

class Goat : public Animal
{ 
public:
    virtual void makesound(){
        cout<<"baaaaa"<<endl;
    }
    Goat() {
        cout<<"you have one more goat"<<endl;
    }
    ~Goat() {
        cout<<"one less goat"<<endl;
    }
    virtual void graze(){
       cout<<"goat eat"<<endl;
    }
    virtual void getmilk(){
       cout<<"have goat milk"<<endl;
   } 
};


class Container 
{ 
    int volume;
    int maxvolume = 100;
    bool full;
    enum type_con {bottle, can, carton};
    type_con type;
 public:
    Container(){
        cout<<"new container"<<endl;
    }
    ~Container(){
        cout<<"~container"<<endl;
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
    void look(){
		cout<<"volume is "<<volume<<endl;
	}
};


class Products 
{ 
public:
   Products(){
       cout<<"new products"<<endl;
   }
   ~Products(){
       cout<<"~products"<<endl;
   }
    
};

class Milk : public Products
{ 
	double fatcon;
    
};



int main(){
    Cow a;
    Animal a1;
    a.makesound();
    a1.graze();
    a.getmilk();
    
    Container c;
    c.sell();
    c.changev();
    c.isfull();
    c.makefull();
    c.look();
    c.isfull();
    c.makeempty();
    c.look();
    c.isfull();
}
