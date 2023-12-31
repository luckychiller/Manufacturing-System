#ifndef QUEUE_H
#define QUEUE_H

typedef struct ItemType {
	int id_;
	int type_;
	double arrivalTime_;
	double interarrivalTime_;

	double delay_;
	double serviceTime_;

	ItemType *next_;
} Item;

class Queue
{
    public:
        Queue();
        inline int& length () { return (length_); }
		void enque (Item *im);
		Item* dque ();

        virtual ~Queue();

    protected:

    private:
        int length_;
		Item *head_;
		Item *tail_;
};

#endif 