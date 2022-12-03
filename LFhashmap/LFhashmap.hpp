#include <iostream>
#include <atomic>
#include <cmath>

#define THRESHOLD 100
static  int HI_MASK = 0x80000000;
static  int MASK = 0x00FFFFFF;

int reverse(int n, size_t b = sizeof(int) * CHAR_BIT )
{
    assert(b <= std::numeric_limits<long>::digits);

    int rv = 0;

    for (size_t i = 0; i < b; ++i, n >>= 1) {
        rv = (rv << 1) | (n & 0x01);
    }

    return rv;
}

int mask(int bucketsize){ return int(pow(2,bucketsize)-1);};

template<class T>
class MarkableReference
{
private:
    uintptr_t val;
    static const uintptr_t mask = 1;
public:
    MarkableReference(T* ref = NULL, bool mark = false)
    {
        val = ((uintptr_t)ref & ~mask) | (mark ? 1 : 0);
    }
    T* getRef(void)
    {
        return (T*)(val & ~mask);
    }

    bool getMark(void)
    {
        return (val & mask);
    }
    T get(bool* mark)
    {
        mark[0]= this->getMark();
        return *this->getRef();
    }
    bool CAS_both(T oldref, T newref, bool oldmark, bool newmark)
    {
        MarkableReference<T> old = *this;
        std::atomic < MarkableReference<T> >tmp;
        tmp.store (old);
        if((old.getRef()->key == oldref.key) and (old.getMark()==oldmark))
          {
            return tmp.compare_exchange_strong(old, MarkableReference(&newref, newmark));
          }
        else
            return false;
    }
};

struct Node{
        int key;
        MarkableReference<Node>* next;
    public:
        Node() {this->key = 0 ;}
        Node(int key) { this->key = key; }
        bool compare_node(Node node1, Node node2){if ( node1.key == node2.key)  {return true;}else{return false;};}
        
};

class Window {
    public: 
    Node pred,curr; 
    Window(Node myPred, Node myCurr) { pred = myPred; curr = myCurr; }
};

Window find(Node head, int key) {
    Node pred , curr , succ ; 
    bool* mark ;
    mark[0]= false;
    bool snip;
    while (true) {
        pred = head;
        curr = *pred.next->getRef(); 
        while (true) {
            succ = curr.next->get(mark); 
            while (mark[0]) {
                snip = pred.next->CAS_both(curr, succ, false, false); 
            if (!snip) continue ;
            curr = succ;
            succ = curr.next->get(mark);
            }
        if (curr.key >= key){return Window(pred, curr);}; 
        pred = curr;
        curr = succ;
        } 
    }
}

class BucketList{ 
    public:
        Node head;
        BucketList() { head = Node(0); head.next = new MarkableReference<Node>(new Node(__INT32_MAX__),false);};
        BucketList(Node node) { head = node ;};
        int makeOrdinaryKey(int key) { key = key & MASK ; return reverse(key | HI_MASK);};
        int makeSentinelKey(int key) { return reverse(key & MASK); };
        bool contains(int x);
        BucketList getSentinel(int index);
        bool add(int item);
        bool remove(int item);
};

bool BucketList::contains(int x) {
    int key = makeOrdinaryKey(x); 
    Window window = find(head, key); 
    Node curr = window.curr;
    return (curr.key == key); 
}
BucketList BucketList::getSentinel(int index) { 
    int key = this->makeSentinelKey(index);
    bool splice;
        while (true) {
            Window window = find(head, key); 
            Node pred = window.pred;
            Node curr = window.curr;
            if (curr.key == key) {
                return BucketList(curr); 
                } 
            else {
                Node node = Node(key); 
                node.next= new MarkableReference<Node>(pred.next->getRef(), false);
                splice = pred.next->CAS_both(curr, node, false, false); 
                if (splice)
                    return BucketList(node); 
                else
                    continue; }
    } 
}
bool BucketList::add(int item) { 
    int key = makeOrdinaryKey(item); 
    while (true) {
        Window window = find(head, key);
        Node pred = window.pred, curr = window.curr; 
        if (curr.key == key) {
            return false;
        }else {
            Node node = Node(item);
            node.next = new MarkableReference<Node>(curr.next->getRef(), false);
            if (pred.next->CAS_both(curr, node, false, false)) {
            return true; 
            }
        } 
    }
}

bool BucketList::remove(int item) { 
    int key = makeOrdinaryKey(item); 
    bool snip;
    while (true) {
        Window window = find(head, key);
        Node pred = window.pred, curr = window.curr; 
        if (curr.key != key) {
            return false;
        }else {
            Node succ = *curr.next->getRef();
            snip = curr.next->CAS_both(succ, succ, false, true); 
            if (!snip) 
                continue;
            pred.next->CAS_both(curr, succ, false, false); 
            return true;
        } 
    }
}

class LockFreeHashSet { 
    private:
        BucketList getBucketList(int myBucket);
        void initializeBucket(int myBucket);
        int getParent(int myBucket);
    public:
        BucketList* bucket; 
        std::atomic<int> bucketSize; 
        std::atomic<int> setSize; 
        LockFreeHashSet(int capacity) {
        bucket = new BucketList[capacity];
        for (int i = 0;i<capacity; i++){
            BucketList* tmp = &bucket[i];
            tmp = NULL ;
        };
        bucket[0] = BucketList(Node(0));
        bucketSize = 2;
        setSize = 0;
        }
        bool add(int key);
        int getbucketSize(){ return bucketSize.load();}
};

BucketList LockFreeHashSet::getBucketList(int myBucket) {
    std::cout<<"bucket[myBucket].head.key:"<<bucket[myBucket].head.key<<std::endl;
    std::cout<<"&bucket[myBucket]"<<&bucket[myBucket]<<std::endl;
    if (&bucket[myBucket] == nullptr)
        initializeBucket(myBucket); 
    return bucket[myBucket];
};

void LockFreeHashSet::initializeBucket(int myBucket) {
    int parent = getParent(myBucket); 
    if (&bucket[parent]==nullptr) 
        initializeBucket(parent);
    BucketList b = bucket[parent].getSentinel(myBucket); 
    if (b.head.key != Node(0).key)
     bucket[myBucket] = b;
};

int LockFreeHashSet::getParent(int myBucket){ 
    int parent = bucketSize.load();
    do {
        parent = parent >> 1;
    } while (parent > myBucket); 
    parent = myBucket - parent; 
    return parent;
};

bool LockFreeHashSet::add(int key) {
    int myBucket = key % bucketSize.load(); 
    BucketList b = getBucketList(myBucket);
    if (!b.add(key))
        return false;
    int setSizeNow = setSize++; 
    int bucketSizeNow = bucketSize.load();
    if ((setSizeNow / bucketSizeNow) > THRESHOLD)
        bucketSize.compare_exchange_strong(bucketSizeNow, 2 * bucketSizeNow); 
    return true;
}

