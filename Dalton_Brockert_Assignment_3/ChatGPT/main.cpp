/*
 * Program Name: EECS 348 Assignment 3
 * Description: C++ program that prioritizes emails for a company CEO using a list-based MaxHeap.
 * Inputs: A test file containing EMAIL, NEXT, READ, and COUNT commands.
 * Outputs: The number of unread emails and information about the next email to be read.
 * Collaborators: None.
 * Other Sources: ChatGPT was used to generate the initial C++ program.
 * Author: Dalton Brockert
 * Creation Date: September 30, 2026
 * Revision Date: September 30, 2026
 * Revisions: Added comments and prologue information to the ChatGPT-generated C++ program.
 */

// Include the iostream library so the program can use terminal input and output.
#include <iostream>

// Include the fstream library so the program can read the input test file.
#include <fstream>

// Include the string library so the program can use string objects.
#include <string>

// Include the sstream library as part of the original generated program.
#include <sstream>

// Allow the program to use standard library names without repeatedly writing std::.
using namespace std;

// Define the Email class to represent an individual email.
class Email {
    
// Make the following data members private so they can only be accessed through the class.
private:
    
    // Store the sender category of the email.
    string sender;
    
    // Store the subject line of the email.
    string subject;
    
    // Store the date of the email.
    string date;
    
    // Store the numerical priority of the sender category.
    int priority;

// Make the following methods public so other objects can use them.
public:
    
    // Define the default constructor for an Email object.
    Email() {
        
        // Initialize the sender as an empty string.
        sender = "";
        
        // Initialize the subject as an empty string.
        subject = "";
        
        // Initialize the date as an empty string.
        date = "";
        
        // Initialize the priority to zero.
        priority = 0;
    }

    // Define a constructor that creates an Email using sender, subject, and date information.
    Email(string sender, string subject, string date) {
        
        // Store the provided sender in the object's sender member.
        this->sender = sender;
        
        // Store the provided subject in the object's subject member.
        this->subject = subject;
        
        // Store the provided date in the object's date member.
        this->date = date;

        // Check whether the sender is the Boss.
        if (sender == "Boss") {
            
            // Give Boss emails the highest priority.
            priority = 5;
        }
        
        // Check whether the sender is a Subordinate.
        else if (sender == "Subordinate") {
            
            // Give Subordinate emails the second-highest priority.
            priority = 4;
        }
        
        // Check whether the sender is a Peer.
        else if (sender == "Peer") {
            
            // Give Peer emails the third-highest priority.
            priority = 3;
        }
        
        // Check whether the sender is an ImportantPerson.
        else if (sender == "ImportantPerson") {
            
            // Give ImportantPerson emails the fourth-highest priority.
            priority = 2;
        }
        
        // Check whether the sender is an OtherPerson.
        else if (sender == "OtherPerson") {
            
            // Give OtherPerson emails the lowest priority.
            priority = 1;
        }
        
        // Handle an unexpected sender category.
        else {
            
            // Give an unknown sender a priority of zero.
            priority = 0;
        }
    }

    // Define a function that returns the sender category.
    string getSender() const {
        
        // Return the sender stored in this Email object.
        return sender;
    }

    // Define a function that returns the subject line.
    string getSubject() const {
        
        // Return the subject stored in this Email object.
        return subject;
    }

    // Define a function that returns the date.
    string getDate() const {
        
        // Return the date stored in this Email object.
        return date;
    }

    // Define a function that returns the priority.
    int getPriority() const {
        
        // Return the priority stored in this Email object.
        return priority;
    }

    // Define a function that converts the date into a numerical value for comparison.
    int getDateValue() const {
        
        // Extract the two-digit month from the date string.
        int month = stoi(date.substr(0, 2));
        
        // Extract the two-digit day from the date string.
        int day = stoi(date.substr(3, 2));
        
        // Extract the four-digit year from the date string.
        int year = stoi(date.substr(6, 4));

        // Convert the date into YYYYMMDD format as an integer and return it.
        return year * 10000 + month * 100 + day;
    }

    // Define a function that determines whether this email has higher priority than another email.
    bool hasHigherPriorityThan(const Email& other) const {
        
        // Check whether the sender priorities are different.
        if (priority != other.priority) {
            
            // Return true when this email has the higher sender priority.
            return priority > other.priority;
        }

        // If sender priorities are equal, compare the dates.
        return getDateValue() > other.getDateValue();
    }
};

// Define the MaxHeap class used as the email priority queue.
class MaxHeap {
    
// Make the heap's internal data private.
private:
    
    // Create a dynamically allocated array of Email objects.
    Email* heap;
    
    // Store the current number of emails in the heap.
    int size;
    
    // Store the current maximum capacity of the heap.
    int capacity;

    // Define a function that doubles the heap's capacity.
    void resize() {
        
        // Double the current capacity.
        capacity *= 2;

        // Allocate a new array with the larger capacity.
        Email* newHeap = new Email[capacity];

        // Copy every existing email into the new array.
        for (int i = 0; i < size; i++) {
            
            // Copy the current email into the new heap.
            newHeap[i] = heap[i];
        }

        // Release the old heap array from memory.
        delete[] heap;
        
        // Make the new array the active heap.
        heap = newHeap;
    }

    // Define a function that swaps two emails in the heap.
    void swapEmails(int first, int second) {
        
        // Store the first email temporarily.
        Email temporary = heap[first];
        
        // Move the second email into the first position.
        heap[first] = heap[second];
        
        // Move the temporary first email into the second position.
        heap[second] = temporary;
    }

    // Move an email upward until the MaxHeap property is restored.
    void heapifyUp(int index) {
        
        // Continue while the email has a parent.
        while (index > 0) {
            
            // Calculate the index of the current email's parent.
            int parent = (index - 1) / 2;

            // Check whether the current email has higher priority than its parent.
            if (heap[index].hasHigherPriorityThan(heap[parent])) {
                
                // Swap the current email with its parent.
                swapEmails(index, parent);
                
                // Continue checking from the parent's previous position.
                index = parent;
            }
            
            // Stop if the MaxHeap property is already satisfied.
            else {
                
                // Exit the heapify-up loop.
                break;
            }
        }
    }

    // Move an email downward until the MaxHeap property is restored.
    void heapifyDown(int index) {
        
        // Continue checking the children until the correct position is found.
        while (true) {
            
            // Calculate the index of the left child.
            int leftChild = 2 * index + 1;
            
            // Calculate the index of the right child.
            int rightChild = 2 * index + 2;
            
            // Initially assume the current element has the highest priority.
            int largest = index;

            // Check whether the left child exists and has higher priority.
            if (leftChild < size &&
                heap[leftChild].hasHigherPriorityThan(heap[largest])) {
                
                // Make the left child the highest-priority element found.
                largest = leftChild;
            }

            // Check whether the right child exists and has higher priority.
            if (rightChild < size &&
                heap[rightChild].hasHigherPriorityThan(heap[largest])) {
                
                // Make the right child the highest-priority element found.
                largest = rightChild;
            }

            // Check whether a child has higher priority than the current element.
            if (largest != index) {
                
                // Swap the current element with the higher-priority child.
                swapEmails(index, largest);
                
                // Continue heapifying from the child's old position.
                index = largest;
            }
            
            // Stop when the current element is already in the correct position.
            else {
                
                // Exit the heapify-down loop.
                break;
            }
        }
    }

// Make the following MaxHeap methods public.
public:
    
    // Define the MaxHeap constructor with a default initial capacity of ten.
    MaxHeap(int initialCapacity = 10) {
        
        // Store the initial capacity.
        capacity = initialCapacity;
        
        // Start the heap with zero emails.
        size = 0;
        
        // Dynamically allocate the initial heap array.
        heap = new Email[capacity];
    }

    // Define the destructor to release dynamically allocated memory.
    ~MaxHeap() {
        
        // Delete the heap array when the MaxHeap object is destroyed.
        delete[] heap;
    }

    // Define the insert function to add an email to the MaxHeap.
    void insert(const Email& email) {
        
        // Check whether the heap is completely full.
        if (size == capacity) {
            
            // Increase the heap capacity before inserting the email.
            resize();
        }

        // Place the new email at the end of the heap.
        heap[size] = email;
        
        // Move the new email upward to restore the MaxHeap property.
        heapifyUp(size);
        
        // Increase the number of emails stored in the heap.
        size++;
    }

    // Define the getMax function to retrieve the highest-priority email.
    Email getMax() const {
        
        // Return the email at the root of the MaxHeap.
        return heap[0];
    }

    // Define the removeMax function to remove the highest-priority email.
    void removeMax() {
        
        // Check whether the heap is empty.
        if (size == 0) {
            
            // Do nothing if there are no emails.
            return;
        }

        // Move the final email into the root position.
        heap[0] = heap[size - 1];
        
        // Decrease the number of emails in the heap.
        size--;

        // Check whether the heap still contains emails.
        if (size > 0) {
            
            // Restore the MaxHeap property by moving the root downward.
            heapifyDown(0);
        }
    }

    // Define the getSize function to return the number of emails.
    int getSize() const {
        
        // Return the current number of emails.
        return size;
    }

    // Define the isEmpty function to determine whether the heap is empty.
    bool isEmpty() const {
        
        // Return true when the heap contains zero emails.
        return size == 0;
    }
};

// Define the EmailManager class that manages the CEO's inbox.
class EmailManager {
    
// Keep the email queue private so it is controlled by the EmailManager.
private:
    
    // Create a MaxHeap object to store the CEO's emails.
    MaxHeap emailQueue;

// Make the EmailManager operations public.
public:
    
    // Define a function that creates and adds an email to the queue.
    void addEmail(string sender, string subject, string date) {
        
        // Create an Email object using the supplied information.
        Email email(sender, subject, date);
        
        // Insert the email into the MaxHeap.
        emailQueue.insert(email);
    }

    // Define a function that displays the next email without removing it.
    void nextEmail() const {
        
        // Check whether there are no emails in the queue.
        if (emailQueue.isEmpty()) {
            
            // Do nothing when there are no emails.
            return;
        }

        // Retrieve the highest-priority email without removing it.
        Email email = emailQueue.getMax();

        // Display the required heading.
        cout << "Next email:" << endl;
        
        // Display the sender category.
        cout << "Sender: " << email.getSender() << endl;
        
        // Display the subject line.
        cout << "Subject: " << email.getSubject() << endl;
        
        // Display the email date.
        cout << "Date: " << email.getDate() << endl;
    }

    // Define a function that removes the highest-priority email.
    void readEmail() {
        
        // Check whether the queue contains an email.
        if (!emailQueue.isEmpty()) {
            
            // Remove the highest-priority email without displaying it.
            emailQueue.removeMax();
        }
    }

    // Define a function that displays the current number of unread emails.
    void countEmails() const {
        
        // Display the number of emails currently stored in the queue.
        cout << "There are " << emailQueue.getSize()
             
             // Finish the required output sentence.
             << " emails to read." << endl;
    }

    // Define a function that processes one command from the input file.
    void processCommand(string command) {
        
        // Check whether the command is COUNT.
        if (command == "COUNT") {
            
            // Display the current number of unread emails.
            countEmails();
        }
        
        // Check whether the command is NEXT.
        else if (command == "NEXT") {
            
            // Display the highest-priority email without removing it.
            nextEmail();
        }
        
        // Check whether the command is READ.
        else if (command == "READ") {
            
            // Remove the highest-priority email.
            readEmail();
        }
        
        // Check whether the command begins with EMAIL followed by a space.
        else if (command.rfind("EMAIL ", 0) == 0) {
            
            // Remove the "EMAIL " portion of the command.
            string emailData = command.substr(6);

            // Find the comma separating the sender and subject.
            size_t firstComma = emailData.find(',');
            
            // Find the comma separating the subject and date.
            size_t secondComma = emailData.find(',', firstComma + 1);

            // Make sure both commas were found.
            if (firstComma != string::npos &&
                secondComma != string::npos) {

                // Extract the sender category from the command.
                string sender = emailData.substr(0, firstComma);
                
                // Extract the subject between the two commas.
                string subject = emailData.substr(
                    firstComma + 1,
                    secondComma - firstComma - 1
                );
                
                // Extract the date after the second comma.
                string date = emailData.substr(secondComma + 1);

                // Add the parsed email to the priority queue.
                addEmail(sender, subject, date);
            }
        }
    }
};

// Define the main function and receive command-line arguments.
int main(int argc, char* argv[]) {
    
    // Check whether an input filename was provided.
    if (argc < 2) {
        
        // Return an error code when no input file was provided.
        return 1;
    }

    // Open the input file specified by the command-line argument.
    ifstream inputFile(argv[1]);

    // Check whether the input file was successfully opened.
    if (!inputFile.is_open()) {
        
        // Return an error code when the file could not be opened.
        return 1;
    }

    // Create an EmailManager object to manage the inbox.
    EmailManager manager;
    
    // Create a string to store each command read from the file.
    string command;

    // Read commands from the file one line at a time.
    while (getline(inputFile, command)) {
        
        // Send the current command to the EmailManager for processing.
        manager.processCommand(command);
    }

    // Close the input file after all commands have been processed.
    inputFile.close();

    // Return zero to indicate successful program completion.
    return 0;
}