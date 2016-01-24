// Constructor with parameters
template <typename T>
Hashtable<T>::Hashtable() : size_(1024) {
    table_ = new std::vector<HashElement<T> >[size_];
}

template <typename T>
Hashtable<T>::Hashtable(unsigned int table_size) : size_(table_size) {
    table_ = new std::vector<HashElement<T> >[size_];
}

// Destructor
template <typename T>
Hashtable<T>::~Hashtable() {
    if(table_) {
        delete [] table_;
    }
}

template <typename T>
unsigned int Hashtable<T>::size() const {
    return size_;
}

template <typename T>
bool Hashtable<T>::contains(const T & value) const {
    bool found = false;
    unsigned long int   hash_code = hashCode(value), // Get hash code
                        i = 0;
    while(i < table_[hash_code].size() && table_[hash_code][i].getValue()!=value) {
        i++;
    }
    if(table_[hash_code].size()>i && table_[hash_code][i].getValue()==value) {
        found = true;
    }
    return found;
}

// Create a hash code, using a string                       /// ONLY for strings
template <typename T>                                       /// TODO: Make a general method
unsigned long int Hashtable<T>::hashCode(T key) const {
    // Using dj2b hash method
    unsigned long code = 5381;

    for(unsigned int i = 0; i < key.length(); i++) {
        code = ((code << 5) + code) + key[i];
    }

    return (code%size_); // To make sure we are in range
}

template <typename T>
bool Hashtable<T>::add(const T & value) {
    unsigned long int   hash_code = hashCode(value), // Get hash code
                        i = 0;

    while(i < table_[hash_code].size() && table_[hash_code][i].getValue()!=value) {
        i++;
    }
    if(table_[hash_code].size()==i) {
        table_[hash_code].push_back(HashElement<T>(value, hash_code));
    }
    table_[hash_code][i]++;

    return true;
}

template <typename T>
bool Hashtable<T>::erase(const T & value) {
    unsigned long int   hash_code = hashCode(value), // Get hash code
                        i = 0;
    bool success = false;

    while(i < table_[hash_code].size() && table_[hash_code][i].getValue()!=value) {
        i++;
    }
    if(table_[hash_code].size()!=i) {
        table_[hash_code].erase(table_[hash_code].begin()+i);
        success = true;
    }

    return success;
}

// Add an element in the right place
template <typename T>
unsigned long int Hashtable<T>::addElement(HashElement<T> * to_add) {
    unsigned long int hash_code = hashCode(to_add->getValue()); // Get hash code
    unsigned int i = 0;
    table_[hash_code] = to_add;

    while(table_[hash_code][i])

    to_add->setKey(hash_code); // Change the key for the element

//std::cout << "Ajout de l'element " << to_add->getValue() << " a la place " << to_add->getKey() << std::endl;

    return to_add->getKey(); // In case we need it
}

// Print everything from the table if not null
template <typename T>
void Hashtable<T>::printAll() const {
    std::cout << "-- TABLE CONTENT --\n" << std::endl;
    for(unsigned long int i = 0 ; i < size_ ; i++) {
        for(unsigned long int j = 0; j < table_[i].size(); j++) { // Each vector
            std::cout << table_[i].at(j).getValue() << "\t#" << table_[i].at(j).number() << std::endl;
        }
    }
    std::cout << "\n-- END OF TABLE --" << std::endl;
}

template <typename T>
std::string Hashtable<T>::toString() const {
    std::string out;
    out += "-- Table --\n";

    for(auto it = std::begin(sorted_); it!=std::end(sorted_) ; ++it) {
        if((*it).getValue().size() > 0)
            out += ((*it).getValue()) + "\t#" + std::string(""+(*it).number()) + "\n";
    }

    return out;
}

/**
 * @brief Give a sorted list of the words and its count
 */
template <typename T>
std::vector<std::string> Hashtable<T>::stringList() const {
    std::vector<std::string> list;
    list.reserve(std::distance(std::begin(sorted_), std::end(sorted_))); // Reserve la taille pour aller plus vite
    std::string buff;

    /// For each words in the sorted list, creates its string
    for(auto it = std::begin(sorted_); it!=std::end(sorted_) ; it++) {
        buff = ((*it).getValue()) + "\t#" + std::string(""+(*it).number()); // "word" #<n>
        list.push_back(buff);
    }

    return list;
}

template <typename T>
std::string Hashtable<T>::sort() {
    std::string result;
    sorted_.push_front(HashElement<T>());
    for(unsigned int i = 0 ; i < size_ ; ++i) {
        for(unsigned int j = 0 ; j < table_[i].size() ; ++j) {
            typename std::forward_list<HashElement<T> >::iterator it = std::begin(sorted_);
            typename std::forward_list<HashElement<T> >::iterator pit = it;
            while(it!=sorted_.end() && (*it).number() < table_[i][j].number()) {
                pit = it;
                it++;
            }
            sorted_.emplace_after(pit, table_[i][j]);
        }
    }
/*
    for(auto it = std::begin(sorted_); it!=std::end(sorted_) ; ++it) {
        if((*it).getValue().size() > 0)
            std::cout << (*it).getValue() << "\t#" << (*it).number() << std::endl;
    }
*/
    return result;
}

template <typename T>
HashElement<T> * Hashtable<T>::search(const T & value) const {
    HashElement<T> * result = NULL;
    unsigned long int   hash_code = hashCode(value), // Get hash code
                        i = 0;
    while(i < table_[hash_code].size() && table_[hash_code][i].getValue()!=value) {
        i++;
    }
    if(table_[hash_code].size()>i && table_[hash_code][i].getValue()==value) {
        result = &(table_[hash_code][i]);
    }
    return result;
}

template <typename T>
bool Hashtable<T>::increase(HashElement<T> * he, long val) {
    bool success = false;
    if(he!=NULL) {
        he->setNumber(val+(he->number()));
        success = true;
    }
    return success;
}

template <typename T>
bool Hashtable<T>::increase(const T & key, long val) {
    bool success = false;
    HashElement<T> * result = search(key);
    if(result!=NULL) {
        if(increase(result, val))
            success = true;
    }
    return success;
}

template <typename T>
void Hashtable<T>::deletePlurals() {
    for(unsigned int i = 0 ; i < size_ ; ++i) {
        for(unsigned int j = 0 ; j < table_[i].size() ; ++j) {
            HashElement<T> word = table_[i][j];
            if(word.getValue().c_str()[word.getValue().size()-1] == 's' || word.getValue().c_str()[word.getValue().size()-1] == 'x') {
                std::string singulier = word.getValue().substr(0,word.getValue().length()-1);
                if(increase(singulier, word.number())) {
                    erase(word.getValue());
                }
            }
        }
    }
}
