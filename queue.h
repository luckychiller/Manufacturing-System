#ifndef queue_h
#define queue_h

typedef struct ItemType {
	int id_;
	ItemType *next_;
} Item;


class Queue 
{
	public:
		Queue ()
        {
            head_ = tail_ = 0;
            length_ = 0;
        }
		inline int& length () 
        { 
            return (length_); 
        }
		void enque (Item *im)
        {
            Item* it = tail_;

            if (!tail_) {
                head_ = tail_ = im;
            } 
            else {
                tail_->next_ = im;
                tail_ = im;
            }

            tail_->next_ = 0;
            ++length_;
        }
		Item* deque ()
        {
            if (!head_)
                return 0;

            Item* it = head_;
            head_ = it->next_;
            if (it == tail_)
                head_ = tail_ = 0;
            --length_;

            return it;
        }
		
	private:
		int length_;
		Item *head_;
		Item *tail_;
};

#endif    