 /*
 * Program Name: EECS 348 Assignment 3
 * Description: C++ program that prioritizes emails for a company CEO using a list-based MaxHeap.
 * Inputs: A test file containing EMAIL, NEXT, READ, and COUNT commands.
 * Outputs: The number of unread emails and information about the next email to be read.
 * Collaborators: None.
 * Other Sources: Gemini was used to generate the initial C++ program.
 * Author: Dalton Brockert
 * Creation Date: September 30, 2026
 * Revision Date: September 30, 2026
 * Revisions: Added comments and prologue information to the Gemini-generated C++ program.
 */


#include <iostream> // For std::cin and std::cout operations
#include <fstream>  // For reading test command input files
#include <string>   // For std::string object operations
#include <sstream>  // For parsing comma-separated line input
#include <vector>   // For dynamic list container underlying the heap
#include <stdexcept> // For standard exception error handling

// Class to store email details and handle custom sorting
class Email { // Defines the Email entity class
private: // Encapsulates internal object data
    std::string senderCategory; // Holds sender priority tier string
    std::string subject;        // Holds the email subject line
    std::string dateStr;        // Holds original MM-DD-YYYY date format
    int senderPriority;         // Holds calculated numeric priority rank
    std::string sortableDate;   // Holds converted YYYYMMDD date string

    // Converts sender category string into integer priority
    int calculateSenderPriority(const std::string& sender) const { // Returns integer rank
        if (sender == "Boss") return 5;            // Boss is highest priority rank
        if (sender == "Subordinate") return 4;     // Subordinate is second highest rank
        if (sender == "Peer") return 3;            // Peer is third highest rank
        if (sender == "ImportantPerson") return 2; // ImportantPerson is fourth rank
        if (sender == "OtherPerson") return 1;     // OtherPerson is lowest rank
        return 0;                                  // Fallback rank for unexpected categories
    } // Closes calculateSenderPriority method

    // Converts MM-DD-YYYY into YYYYMMDD string format
    std::string formatSortableDate(const std::string& date) const { // Converts date strings
        if (date.length() != 10) return date; // Guards against invalid string length
        std::string month = date.substr(0, 2); // Extracts month characters
        std::string day = date.substr(3, 2);   // Extracts day characters
        std::string year = date.substr(6, 4);  // Extracts year characters
        return year + month + day;             // Recombines into sortable string
    } // Closes formatSortableDate method

public: // Public constructors and accessors interface
    Email() : senderPriority(0) {} // Default constructor for vector initialization

    // Constructs email and generates comparison values
    Email(const std::string& sender, const std::string& subj, const std::string& date) // Parameterized constructor
        : senderCategory(sender), subject(subj), dateStr(date) {                        // Initializer list
        senderPriority = calculateSenderPriority(sender);                              // Assigns integer priority
        sortableDate = formatSortableDate(date);                                       // Assigns sortable date
    } // Closes parameterized constructor

    std::string getSenderCategory() const { return senderCategory; } // Getter for sender category
    std::string getSubject() const { return subject; }               // Getter for subject line
    std::string getDate() const { return dateStr; }                  // Getter for formatted date string

    // Determines heap ordering (sender category first, newest date second)
    bool operator<(const Email& other) const { // Overloads less-than operator
        if (this->senderPriority != other.senderPriority) { // Checks if categories differ
            return this->senderPriority < other.senderPriority; // Ranks lower priority integer lower
        } // Closes category comparison block
        return this->sortableDate < other.sortableDate; // Ranks older date string lower
    } // Closes operator< method

    bool operator>(const Email& other) const { // Overloads greater-than operator
        return other < *this;                  // Delegates through operator<
    } // Closes operator> method
}; // Closes Email class definition

// Custom list-based MaxHeap priority queue
class MaxHeap { // Defines the MaxHeap class
private: // Encapsulates internal tree structures
    std::vector<Email> heap; // Dynamic list array backing tree structure

    int parent(int i) const { return (i - 1) / 2; }     // Computes parent array index
    int leftChild(int i) const { return (2 * i) + 1; }  // Computes left child index
    int rightChild(int i) const { return (2 * i) + 2; } // Computes right child index

    // Restores max heap order from index i up to root
    void heapifyUp(int i) { // Sifts element upward
        while (i > 0 && heap[parent(i)] < heap[i]) { // Loops while current exceeds parent
            std::swap(heap[i], heap[parent(i)]);      // Swaps current element with parent
            i = parent(i);                            // Moves tracking index to parent
        } // Closes sift-up loop
    } // Closes heapifyUp method

    // Restores max heap order from index i down to leaf
    void heapifyDown(int i) { // Sifts element downward
        int maxIndex = i;     // Tracks index of highest priority node
        int left = leftChild(i);   // Gets left child node index
        int right = rightChild(i); // Gets right child node index

        if (left < static_cast<int>(heap.size()) && heap[maxIndex] < heap[left]) { // Validates left child
            maxIndex = left; // Marks left child as largest
        } // Closes left child check
        if (right < static_cast<int>(heap.size()) && heap[maxIndex] < heap[right]) { // Validates right child
            maxIndex = right; // Marks right child as largest
        } // Closes right child check

        if (i != maxIndex) {                       // Checks if a child was larger
            std::swap(heap[i], heap[maxIndex]);    // Swaps root with largest child
            heapifyDown(maxIndex);                 // Recursively sifts down next level
        } // Closes swap conditional block
    } // Closes heapifyDown method

public: // Public operations interface
    MaxHeap() {} // Default constructor

    // Inserts element and fixes heap property
    void insert(const Email& email) {               // Inserts new element
        heap.push_back(email);                      // Appends element to list end
        heapifyUp(static_cast<int>(heap.size()) - 1); // Bubbles appended element up
    } // Closes insert method

    // Reads highest priority email without removal
    Email peek() const { // Retrieves top node
        if (heap.empty()) { // Checks if heap has elements
            throw std::runtime_error("Heap is empty."); // Throws runtime error if empty
        } // Closes empty heap check
        return heap[0]; // Returns element at root index
    } // Closes peek method

    // Removes highest priority email from root
    void pop() { // Removes top element
        if (heap.empty()) return; // Guards against popping empty heap
        
        heap[0] = heap.back(); // Overwrites root with last element
        heap.pop_back();       // Removes duplicate element from back
        if (!heap.empty()) {   // Checks if elements remain in heap
            heapifyDown(0);    // Sifts new root downward
        } // Closes non-empty check block
    } // Closes pop method

    size_t size() const { return heap.size(); } // Returns current element count
    bool empty() const { return heap.empty(); } // Returns true if heap has no items
}; // Closes MaxHeap class definition

// Parses test commands and delegates email system handling
class EmailPriorityQueue { // Defines manager class
private: // Encapsulates system queue object
    MaxHeap priorityQueue; // Instantiate custom max heap structure

public: // Public command interface
    // Parses incoming EMAIL command strings
    void processEmailCommand(const std::string& line) { // Handles email creation
        std::string data = line.substr(6); // Strips leading "EMAIL " prefix
        std::stringstream ss(data);        // Places line data in stream
        std::string sender, subject, date; // Declares string parameter containers

        if (std::getline(ss, sender, ',') &&   // Extracts sender substring token
            std::getline(ss, subject, ',') &&  // Extracts subject substring token
            std::getline(ss, date, ',')) {     // Extracts date substring token
            
            Email newEmail(sender, subject, date); // Constructs new email object
            priorityQueue.insert(newEmail);        // Adds constructed email to heap
        } // Closes delimiter extraction block
    } // Closes processEmailCommand method

    // Displays top unread email details
    void handleNext() { // Implements NEXT command
        if (priorityQueue.empty()) return; // Skips execution if queue empty

        Email topEmail = priorityQueue.peek(); // Retrieves top email object
        std::cout << "Next email:\n"; // Outputs next email header
        std::cout << "Sender: " << topEmail.getSenderCategory() << "\n"; // Outputs sender category
        std::cout << "Subject: " << topEmail.getSubject() << "\n";       // Outputs email subject line
        std::cout << "Date: " << topEmail.getDate() << "\n";             // Outputs formatted email date
    } // Closes handleNext method

    // Removes top unread email from queue
    void handleRead() { // Implements READ command
        if (!priorityQueue.empty()) { // Checks if queue holds items
            priorityQueue.pop();      // Deletes highest priority email node
        } // Closes non-empty check
    } // Closes handleRead method

    // Prints number of pending emails
    void handleCount() { // Implements COUNT command
        std::cout << "There are " << priorityQueue.size() << " emails to read.\n"; // Outputs email count
    } // Closes handleCount method
}; // Closes EmailPriorityQueue class definition

int main(int argc, char* argv[]) { // Execution entry point
    std::istream* inputSource = &std::cin; // Default input source set to std::cin
    std::ifstream fileStream;              // Declares optional input file stream

    if (argc < 2) { // Checks if an input file path was provided
        std::cerr << "Error: An input file is required.\n"; // Reports missing input file
        return 1; // Exits with a nonzero status for missing input file
    } // Closes missing argument check

    fileStream.open(argv[1]); // Opens path provided in argument
    if (!fileStream.is_open()) { // Verifies file opened successfully
        std::cerr << "Error: Could not open input file.\n"; // Reports file opening error
        return 1; // Exits with a nonzero status for file opening failure
    } // Closes file open verification block

    inputSource = &fileStream; // Redirects input stream to file

    EmailPriorityQueue emailSystem; // Instantiates email manager system object
    std::string line; // Declares buffer variable for input lines

    while (std::getline(*inputSource, line)) { // Loops through command input lines
        if (line.empty()) continue; // Ignores empty input lines

        if (line.rfind("EMAIL ", 0) == 0) { // Matches lines starting with "EMAIL "
            emailSystem.processEmailCommand(line); // Routes to EMAIL handler
        } else if (line == "NEXT") { // Matches "NEXT" command literal
            emailSystem.handleNext(); // Routes to NEXT handler
        } else if (line == "READ") { // Matches "READ" command literal
            emailSystem.handleRead(); // Routes to READ handler
        } else if (line == "COUNT") { // Matches "COUNT" command literal
            emailSystem.handleCount(); // Routes to COUNT handler
        } // Closes command decision tree
    } // Closes command reading loop

    return 0; // Signals successful execution to system
} // Closes main function definition