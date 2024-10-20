#ifndef MANAGER_FUNCTIONS
#define MANAGER_FUNCTIONS

#include "./common.h"

struct Employee loggedInManager;

// Function Prototypes =================================

bool manager_operation_handler(int connFD);
bool get_feedback_details(int connFD);
bool delete_customer_account(int connFD);
bool get_loans_details(int connFD);
bool assign_loan(int connFD);
bool change_manager_password(int connFD);



bool manager_operation_handler(int connFD)
{
    if(login_employee_manager_handler(3, connFD, &loggedInManager))
    {
        ssize_t writeBytes, readBytes;            // Number of bytes read from/written to the client
        char readBuffer[1000], writeBuffer[1000]; // A buffer used for reading & writing to the client
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "Welcome Banker!\n");

        while (1)
        {
            strcat(writeBuffer, "1. Activate/Deactivate Customer Accounts \n2. Assign Loan Application Processes to Employees \n3. Review Customer Feedback \n4. Change Password \n5. View All Loans \nPress any other key to logout");
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));

            bzero(writeBuffer, sizeof(writeBuffer));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));

            int choice = atoi(readBuffer);
            switch (choice)
            {
            case 1:
                delete_customer_account(connFD);
                //get_customer_details(connFD, -1);
                break;
            case 2:
                assign_loan(connFD);
                //get_account_details(connFD, NULL);
                break;
            case 3: 
                get_feedback_details(connFD);
                //get_transaction_details(connFD, -1);
                break;
            case 4:
                change_manager_password(connFD);
                break;
            case 5:
                get_loans_details(connFD);
                //delete_account(connFD);
                break;
            case 6:
                writeBytes = write(connFD, "Logging you out now superman! Goodbye!$", strlen("Logging you out now superman! Goodbye!$"));
                return false;
            default:
                writeBytes = write(connFD, "Logging you out now superman! Goodbye!$", strlen("Logging you out now superman! Goodbye!$"));
                return false;
            }
        }
    }
    else
    {
        // ADMIN LOGIN FAILED
        return false;
    }
    return true;
}

bool change_manager_password(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000], hashedPassword[1000];

    char newPassword[1000];

    writeBytes = write(connFD, "Enter your old password", strlen("Enter your old password"));
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));

    if (strcmp(crypt(readBuffer, SALT_BAE), loggedInManager.password) == 0)
    {
        // Password matches with old password
        writeBytes = write(connFD, "Enter the new password", strlen("Enter the new password"));
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        
        //hasing new password
        strcpy(newPassword, crypt(readBuffer, SALT_BAE));

        writeBytes = write(connFD, "Reenter the new password", strlen("Reenter the new password"));
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));

        if (strcmp(crypt(readBuffer, SALT_BAE), newPassword) == 0)
        {
           strcpy(loggedInManager.password, newPassword);

            int employeeFileDescriptor = open(EMPLOYEE_FILE, O_WRONLY);

            off_t offset = lseek(employeeFileDescriptor, loggedInManager.id * sizeof(struct Employee), SEEK_SET);
            if (offset == -1)
            {
                perror("Error seeking to the manager record!");
                return false;
            }

            struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Employee), getpid()};
            int lockingStatus = fcntl(employeeFileDescriptor, F_SETLKW, &lock);

            writeBytes = write(employeeFileDescriptor, &loggedInManager, sizeof(struct Employee));
            if (writeBytes == -1)
            {
                perror("Error storing updated employee password into employee record!");
                return false;
            }

            lock.l_type = F_UNLCK;
            lockingStatus = fcntl(employeeFileDescriptor, F_SETLK, &lock);
            close(employeeFileDescriptor);

            writeBytes = write(connFD, "Password successfully changed!^", strlen("Password successfully changed!^"));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
            return true;
        }
        else
        {
            // New & reentered passwords don't match
            writeBytes = write(connFD, "New password and Reentered password don't match!^", strlen("New password and Reentered password don't match!^"));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        }
    }
    else
    {
        // Password doesn't match with old password
        writeBytes = write(connFD, "The entered password doesn't match with the old password!^", strlen("The entered password doesn't match with the old password!^"));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
    }
    return false;
}

bool assign_loan(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Loan loan;

    writeBytes = write(connFD, "What is the loan ID you want to assign?", strlen("What is the loan ID you want to assign?"));
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));

    int loanNumber = atoi(readBuffer);

    int loanFileDescriptor = open(LOAN_FILE, O_RDONLY);
    if (loanFileDescriptor == -1)
    {
        // loan file doesn't exist
        bzero(readBuffer, sizeof(readBuffer));
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "No loan file exists^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }


    int offset = lseek(loanFileDescriptor, loanNumber * sizeof(struct Loan), SEEK_SET);
    if (errno == EINVAL)
    {
        // Loan Number doesn't exist
        bzero(readBuffer, sizeof(readBuffer));
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "No loan could be found for the given loan number^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    else if (offset == -1)
    {
        perror("Error while seeking to required loan!");
        return false;
    }

    //applying lock
    struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Loan), getpid()};
    fcntl(loanFileDescriptor, F_SETLKW, &lock);
    readBytes = read(loanFileDescriptor, &loan, sizeof(struct Loan));
    //unlocking
    lock.l_type = F_UNLCK;
    fcntl(loanFileDescriptor, F_SETLK, &lock);
    close(loanFileDescriptor);

    bzero(writeBuffer, sizeof(writeBuffer));
    bzero(readBuffer, sizeof(readBuffer));

    if(loan.EmployeeID!=-1 || loan.approved!=0)
    {
        if(loan.approved!=0)
        strcpy(writeBuffer, "The loan cannot be assigned since it is already processed\nRedirecting to the main menu...^");
        else if(loan.EmployeeID!=-1)
        strcpy(writeBuffer, "The loan cannot be assigned since it is already assigned\nRedirecting to the main menu...^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }


    struct Employee receiverEmployee;
    writeBytes = write(connFD, "Enter the Employee number you want to assign", strlen("Enter the Employee number you want to assign"));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    receiverEmployee.id = atoi(readBuffer);

    if(get_employee_details(connFD, &receiverEmployee))
    {
        if(receiverEmployee.loanID[MAX_LOANS-1]==-1)
        {
            loan.EmployeeID=receiverEmployee.id;
            //update in loan file
            loanFileDescriptor = open(LOAN_FILE, O_WRONLY);

            offset = lseek(loanFileDescriptor, loanNumber * sizeof(struct Loan), SEEK_SET);
            lock.l_type = F_WRLCK;
            lock.l_start = offset;
            fcntl(loanFileDescriptor, F_SETLKW, &lock);
            writeBytes = write(loanFileDescriptor, &loan, sizeof(struct Loan));
            lock.l_type = F_UNLCK;
            fcntl(loanFileDescriptor, F_SETLK, &lock);

            close(loanFileDescriptor);

            int iter = 0;
            while (receiverEmployee.loanID[iter] != -1)
                iter++;
            receiverEmployee.loanID[iter] = loan.loanID;  // Space available

            int employeeFileDescriptor = open(EMPLOYEE_FILE, O_WRONLY);

            offset = lseek(employeeFileDescriptor, receiverEmployee.id * sizeof(struct Employee), SEEK_SET);
            lock.l_type = F_WRLCK;
            lock.l_start = offset;
            fcntl(employeeFileDescriptor, F_SETLKW, &lock);
            writeBytes = write(employeeFileDescriptor, &receiverEmployee, sizeof(struct Employee));
            lock.l_type = F_UNLCK;
            fcntl(employeeFileDescriptor, F_SETLK, &lock);

            close(employeeFileDescriptor);

            strcpy(writeBuffer, "The loan is assigned \nRedirecting to the main menu...^");
        }
        else
        {
            //already assignment is full for the employee
            strcpy(writeBuffer, "The employee's loan application is full. Please assign to other Employee.\nRedirecting to the main menu...^");
        }
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return true;   
    }
    else
    {
        //receiver employee doesn't exists
        strcpy(writeBuffer, "The receiver employee doesn't exists\nRedirecting to the main menu...^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;  
    }
}

bool get_loans_details(int connFD)
{
    ssize_t readBytes, writeBytes;                               // Number of bytes read from / written to the socket
    char readBuffer[1000], writeBuffer[10000], tempBuffer[1000]; // A buffer for reading from / writing to the socket

    struct Loan loan;

    int iter;
    int number_of_loans=-1;

    bzero(writeBuffer, sizeof(readBuffer));
    bzero(readBuffer, sizeof(readBuffer));
    bzero(tempBuffer, sizeof(tempBuffer));

    int loanFileDescriptor = open(LOAN_FILE, O_RDONLY);
    if (loanFileDescriptor == -1)
    {
        perror("Error while opening feedback file!");
        write(connFD, "No loans were applied!^", strlen("No loans were applied!^"));
        read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }

    // Get most recent feedback number
    off_t offset = lseek(loanFileDescriptor, -sizeof(struct Loan), SEEK_END);
    if (offset >= 0)
    {
        // There exists at least one feedback
        struct Loan prevLoan;

        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Loan), getpid()};
        fcntl(loanFileDescriptor, F_SETLKW, &lock);
        readBytes = read(loanFileDescriptor, &prevLoan, sizeof(struct Loan));
        lock.l_type = F_UNLCK;
        fcntl(loanFileDescriptor, F_SETLK, &lock);
        number_of_loans=prevLoan.loanID;
    }


    for (iter = 0; iter <=number_of_loans; iter++)
    {
        offset = lseek(loanFileDescriptor, iter*sizeof(struct Loan), SEEK_SET);
        if (offset == -1)
        {
            perror("Error while seeking to loans!");
            return false;
        }

        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Loan), getpid()};
        fcntl(loanFileDescriptor, F_SETLKW, &lock);
        readBytes = read(loanFileDescriptor, &loan, sizeof(struct Loan));
        lock.l_type = F_UNLCK;
        fcntl(loanFileDescriptor, F_SETLK, &lock);

        bzero(tempBuffer, sizeof(tempBuffer));
        sprintf(tempBuffer, "Loan ID : %d \n Customer ID : %d \n Amount : %ld \n Approved Status : %s \n Assigned Employee ID : %d \n\n", loan.loanID, loan.customerID , loan.amount, (loan.approved==0? "Pending" : (loan.approved==1? "Approved": "Rejected") ) , loan.EmployeeID);

        if (strlen(writeBuffer) == 0)
            strcpy(writeBuffer, tempBuffer);
        else
            strcat(writeBuffer, tempBuffer);
    }

    close(loanFileDescriptor);

    if (strlen(writeBuffer) == 0)
    {
        write(connFD, "No Loans were found!^", strlen("No Loans were found!^"));
        read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    else
    {
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
    }
}

bool get_feedback_details(int connFD)
{

    ssize_t readBytes, writeBytes;                               // Number of bytes read from / written to the socket
    char readBuffer[1000], writeBuffer[10000], tempBuffer[1000]; // A buffer for reading from / writing to the socket

    struct Feedback feedback;

    int iter;
    int number_of_feedbacks=-1;

    bzero(writeBuffer, sizeof(readBuffer));
    bzero(readBuffer, sizeof(readBuffer));
    bzero(tempBuffer, sizeof(tempBuffer));

    int feedbackFileDescriptor = open(FEEDBACK_FILE, O_RDONLY);
    if (feedbackFileDescriptor == -1)
    {
        perror("Error while opening feedback file!");
        write(connFD, "No feedback were written!^", strlen("No feddback were written!^"));
        read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }

    // Get most recent feedback number
    off_t offset = lseek(feedbackFileDescriptor, -sizeof(struct Feedback), SEEK_END);
    if (offset >= 0)
    {
        // There exists at least one feedback
        struct Feedback prevFeedback;
        readBytes = read(feedbackFileDescriptor, &prevFeedback, sizeof(struct Feedback));
        number_of_feedbacks=prevFeedback.feedbackID;
    }


    for (iter = 0; iter <=number_of_feedbacks; iter++)
    {
        offset = lseek(feedbackFileDescriptor, iter*sizeof(struct Feedback), SEEK_SET);
        if (offset == -1)
        {
            perror("Error while seeking to required feedback!");
            return false;
        }

        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Feedback), getpid()};

        int lockingStatus = fcntl(feedbackFileDescriptor, F_SETLKW, &lock);

        readBytes = read(feedbackFileDescriptor, &feedback, sizeof(struct Feedback));

        lock.l_type = F_UNLCK;
        fcntl(feedbackFileDescriptor, F_SETLK, &lock);

        bzero(tempBuffer, sizeof(tempBuffer));
        sprintf(tempBuffer, "Feedback ID : %d \n Account No. : %d \n Customer Name : %s \n Feedback : %s\n", feedback.feedbackID, feedback.account , feedback.name , feedback.message);

        if (strlen(writeBuffer) == 0)
            strcpy(writeBuffer, tempBuffer);
        else
            strcat(writeBuffer, tempBuffer);
    }

    close(feedbackFileDescriptor);

    if (strlen(writeBuffer) == 0)
    {
        write(connFD, "No feedbacks were found!^", strlen("No feedbacks were found!^"));
        read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    else
    {
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
    }
}

bool delete_customer_account(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Account account;

    writeBytes = write(connFD, "What is the account number of the account you want to check?", strlen("What is the account number of the account you want to check?"));
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));

    int accountNumber = atoi(readBuffer);

    int accountFileDescriptor = open(ACCOUNT_FILE, O_RDONLY);
    if (accountFileDescriptor == -1)
    {
        // Account record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "No account could be found for the given account number^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }


    int offset = lseek(accountFileDescriptor, accountNumber * sizeof(struct Account), SEEK_SET);
    if (errno == EINVAL)
    {
        // Customer record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "No account could be found for the given account number^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    else if (offset == -1)
    {
        perror("Error while seeking to required account record!");
        return false;
    }

    //applying lock
    struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Account), getpid()};
    int lockingStatus = fcntl(accountFileDescriptor, F_SETLKW, &lock);

    readBytes = read(accountFileDescriptor, &account, sizeof(struct Account));
    //unlocking
    lock.l_type = F_UNLCK;
    fcntl(accountFileDescriptor, F_SETLK, &lock);
    close(accountFileDescriptor);


    bzero(writeBuffer, sizeof(writeBuffer));

    writeBytes = write(connFD, "Enter 1. to Deactivate it 2. To Activate it", strlen("Enter 1. to Deactivate it 2. To Activate it"));
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));

    int choice = atoi(readBuffer);

    switch(choice)
    {
        case 1:
        if (account.balance == 0)
        {
            // No money, hence can close account
            account.active = false;
            accountFileDescriptor = open(ACCOUNT_FILE, O_WRONLY);

            offset = lseek(accountFileDescriptor, accountNumber * sizeof(struct Account), SEEK_SET);
            lock.l_type = F_WRLCK;
            lock.l_start = offset;
            fcntl(accountFileDescriptor, F_SETLKW, &lock);

            writeBytes = write(accountFileDescriptor, &account, sizeof(struct Account));

            lock.l_type = F_UNLCK;
            fcntl(accountFileDescriptor, F_SETLK, &lock);

            strcpy(writeBuffer, "The account is deactivated. \nRedirecting to the main menu...^");
        }
        else
            // Account has some money ask customer to withdraw it
            strcpy(writeBuffer, "The account cannot be deactivated since it still has some money\nRedirecting to the main menu...^");
        
        break;

        case 2:
            account.active = true;
            accountFileDescriptor = open(ACCOUNT_FILE, O_WRONLY);

            offset = lseek(accountFileDescriptor, accountNumber * sizeof(struct Account), SEEK_SET);
            lock.l_type = F_WRLCK;
            lock.l_start = offset;
            fcntl(accountFileDescriptor, F_SETLKW, &lock);

            writeBytes = write(accountFileDescriptor, &account, sizeof(struct Account));

            lock.l_type = F_UNLCK;
            fcntl(accountFileDescriptor, F_SETLK, &lock);

            strcpy(writeBuffer, "The account is activated. \nRedirecting to the main menu...^");
        break;
        default:
            writeBytes = write(connFD, "Logging you out now! Goodbye!$", strlen("Logging you out now! Goodbye!$"));
            return false;
    }


    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
    return true;
}


#endif