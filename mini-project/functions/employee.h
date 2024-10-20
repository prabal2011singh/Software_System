#ifndef EMPLOYEE_FUNCTIONS
#define EMPLOYEE_FUNCTIONS

#include "./common.h"

struct Employee loggedInEmployee;
int semIdentifier2;


// Function Prototypes =================================

bool employee_operation_handler(int connFD);
bool add_customer_account(int connFD);
int add_customer(int connFD, int newAccountNumber);
bool modify_customer_info(int connFD);
bool get_assigned_loans(int connFD, int employeeNumber);
bool approve_reject_loans(int connFD, int employeeNumber);
bool loan_deposit(int connFD, int accountNumber, long int amount);
bool change_employee_password(int connFD);

// ========================================================

bool employee_operation_handler(int connFD)
{
    if(login_employee_manager_handler(2, connFD, &loggedInEmployee))
    {
        ssize_t writeBytes, readBytes;            // Number of bytes read from/written to the client
        char readBuffer[1000], writeBuffer[1000]; // A buffer used for reading & writing to the client
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "Welcome Banker!\n");

        while (1)
        {
            strcat(writeBuffer, "1. Add New Customer \n2. Modify Customer Details \n3. Approve/Reject Loans \n4. View Assigned Loans \n5. View Customer Transactions \n6. Change Password \nPress any other key to logout");
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));

            bzero(writeBuffer, sizeof(writeBuffer));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));

            int choice = atoi(readBuffer);
            switch (choice)
            {
            case 1:
                add_customer_account(connFD);  //first account number is created then customer details are taken and customer is created and then account is created
                break;
            case 2:
                modify_customer_info(connFD);
                break;
            case 3: 
                approve_reject_loans(connFD,loggedInEmployee.id);
                //get_transaction_details(connFD, -1);
                break;
            case 4:
                get_assigned_loans(connFD, loggedInEmployee.id);
                //get_account_details(connFD, NULL);
                break;
            case 5:
                get_transaction_details(connFD,-1);
                //delete_account(connFD);
                break;
            case 6:
                change_employee_password(connFD);
                //get_customer_details(connFD, -1);
                break;
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

bool change_employee_password(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000], hashedPassword[1000];

    char newPassword[1000];

    writeBytes = write(connFD, "Enter your old password", strlen("Enter your old password"));
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));

    if (strcmp(crypt(readBuffer, SALT_BAE), loggedInEmployee.password) == 0)
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
           strcpy(loggedInEmployee.password, newPassword);

            int employeeFileDescriptor = open(EMPLOYEE_FILE, O_WRONLY);

            off_t offset = lseek(employeeFileDescriptor, loggedInEmployee.id * sizeof(struct Employee), SEEK_SET);
            if (offset == -1)
            {
                perror("Error seeking to the employee record!");
                return false;
            }

            struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Employee), getpid()};
            int lockingStatus = fcntl(employeeFileDescriptor, F_SETLKW, &lock);

            writeBytes = write(employeeFileDescriptor, &loggedInEmployee, sizeof(struct Employee));
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

bool approve_reject_loans(int connFD, int employeeNumber)
{
    ssize_t readBytes, writeBytes;                               // Number of bytes read from / written to the socket
    char readBuffer[1000], writeBuffer[1000], tempBuffer[1000]; // A buffer for reading from / writing to the socket
    struct Employee employee;

    bzero(writeBuffer, sizeof(writeBuffer));
    bzero(readBuffer, sizeof(readBuffer));
    bzero(tempBuffer, sizeof(tempBuffer));

    employee.id = employeeNumber;

    if(get_employee_details(connFD, &employee))
    {
        if(employee.loanID[0]!=-1)
        {
            struct Loan loan;

            //read one loan from assigned loans
            int loanFileDescriptor = open(LOAN_FILE, O_RDONLY);
            off_t offset = lseek(loanFileDescriptor, employee.loanID[0] * sizeof(struct Loan), SEEK_SET);

            //locking
            struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Loan), getpid()};
            fcntl(loanFileDescriptor, F_SETLKW, &lock);

            readBytes = read(loanFileDescriptor, &loan, sizeof(struct Loan));

            //unlocking
            lock.l_type = F_UNLCK;
            fcntl(loanFileDescriptor, F_SETLK, &lock);
            close(loanFileDescriptor);

            //storing loan in tempbuffer to display
            sprintf(tempBuffer, "Loan ID : %d \n Customer ID : %d \n Amount : %ld \n Approved Status : %s \n Assigned Employee ID : %d \n\n", loan.loanID, loan.customerID , loan.amount, (loan.approved==0? "Pending" : (loan.approved==1? "Approved": "Rejected") ) , loan.EmployeeID);

            strcpy(writeBuffer, tempBuffer);
            strcat(writeBuffer, "\n\n");
            strcat(writeBuffer, "Enter 1. to Approve the loan or 2. To Reject the loan");
            
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));

            int choice = atoi(readBuffer);

            bzero(writeBuffer, sizeof(writeBuffer));
            bzero(readBuffer, sizeof(readBuffer));
            switch (choice)
            {
            case 1:
                loan.approved=1;
                // make a function to deposit money in customer's account
                loan_deposit(connFD, loan.customerID, loan.amount);
                break;
            case 2:
                loan.approved=-1;
                break;
            default:
                strcpy(writeBuffer, "You have entered an invalid number! \nYou'll now be redirected to the main menu!^");
                writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
                readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
                return false;
            }

            //update in loan file
            loanFileDescriptor = open(LOAN_FILE, O_WRONLY);
            offset = lseek(loanFileDescriptor, employee.loanID[0] * sizeof(struct Loan), SEEK_SET);

            lock.l_type = F_WRLCK;
            lock.l_start = offset;
            fcntl(loanFileDescriptor, F_SETLKW, &lock);
            writeBytes = write(loanFileDescriptor, &loan, sizeof(struct Loan));
            lock.l_type = F_UNLCK;
            fcntl(loanFileDescriptor, F_SETLKW, &lock);
            close(loanFileDescriptor);

            //update the loan array of employee
            int curr_present_loans=0;
            while(curr_present_loans<MAX_LOANS && employee.loanID[curr_present_loans]!=-1)
            curr_present_loans++;

            for(int i=1;i<curr_present_loans;i++)
                employee.loanID[i-1]=employee.loanID[i];
            
            employee.loanID[curr_present_loans-1]=-1;

            //update in employee file
            int employeeFileDescriptor = open(EMPLOYEE_FILE, O_WRONLY);
            off_t offset2 = lseek(employeeFileDescriptor, employee.id * sizeof(struct Employee), SEEK_SET);
            struct flock lock2 = {F_WRLCK, SEEK_SET, offset2, sizeof(struct Employee), getpid()};
            fcntl(employeeFileDescriptor, F_SETLKW, &lock2);
            writeBytes = write(employeeFileDescriptor, &employee, sizeof(struct Employee));
            lock2.l_type = F_UNLCK;
            fcntl(employeeFileDescriptor, F_SETLKW, &lock2);
            close(employeeFileDescriptor);


            writeBytes = write(connFD, "The Loan processing was successfully done! \nYou'll now be redirected to the main menu!^", strlen("The Loan processing was successfully done! \nYou'll now be redirected to the main menu!^"));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
            return true;
        }
        else
        {
            //no loans assigned
            write(connFD, "No loans are presently assigned!^", strlen("No loans are presently assigned!^"));
            read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
            return false;
        }
    }
}

bool loan_deposit(int connFD, int accNumber, long int amount)
{
    char readBuffer[1000], writeBuffer[1000];
    ssize_t readBytes, writeBytes;
    struct Account account;
    account.accountNumber = accNumber;

    long int depositAmount = amount;

    if (get_account_details(connFD, &account))
    {
        if (account.active)
        {
                int newTransactionID = write_transaction_to_file(account.accountNumber, account.balance, account.balance + depositAmount, 1);
                write_transaction_to_array(account.transactions, newTransactionID);

                account.balance += depositAmount;

                //updating in the balance in account file
                int accountFileDescriptor = open(ACCOUNT_FILE, O_WRONLY);
                off_t offset3 = lseek(accountFileDescriptor, account.accountNumber * sizeof(struct Account), SEEK_SET);

                struct flock lock3 = {F_WRLCK, SEEK_SET, offset3, sizeof(struct Account), getpid()};
                fcntl(accountFileDescriptor, F_SETLKW, &lock3);

                writeBytes = write(accountFileDescriptor, &account, sizeof(struct Account));
                if (writeBytes == -1)
                {
                    perror("Error storing updated deposit money in account record!");
                    return false;
                }

                lock3.l_type = F_UNLCK;
                fcntl(accountFileDescriptor, F_SETLK, &lock3);

                write(connFD, "The loan amount has been successfully added!^", strlen("The loan amount has been successfully added!^"));
                read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
                return true;
        }
        else
            write(connFD, "It seems the account has been deactivated!^", strlen("It seems the account has been deactivated!^"));

        read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    else
    {
        // FAIL
        return false;
    }
}

bool get_assigned_loans(int connFD, int employeeNumber)
{

    ssize_t readBytes, writeBytes;                               // Number of bytes read from / written to the socket
    char readBuffer[1000], writeBuffer[10000], tempBuffer[1000]; // A buffer for reading from / writing to the socket
    struct Employee employee;

    employee.id = employeeNumber;

    if (get_employee_details(connFD, &employee))
    {
        int iter;
        struct Loan loan;

        bzero(writeBuffer, sizeof(writeBuffer));

        int loanFileDescriptor = open(LOAN_FILE, O_RDONLY);
        if (loanFileDescriptor == -1)
        {
            perror("Error while opening loan file!");
            write(connFD, "No loan were present on this bank!^", strlen("No loan were present on this bank!^"));
            read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
            return false;
        }

        for (iter = 0; iter < MAX_LOANS && employee.loanID[iter] != -1; iter++)
        {
            int offset = lseek(loanFileDescriptor, employee.loanID[iter] * sizeof(struct Loan), SEEK_SET);

            struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Loan), getpid()};
            fcntl(loanFileDescriptor, F_SETLKW, &lock);

            readBytes = read(loanFileDescriptor, &loan, sizeof(struct Loan));

            lock.l_type = F_UNLCK;
            fcntl(loanFileDescriptor, F_SETLK, &lock);

            bzero(tempBuffer, sizeof(tempBuffer));
            sprintf(tempBuffer, "Loan ID : %d \n Customer ID : %d \n Amount : %ld \n Approved Status : %s \n Assigned Employee ID : %d \n\n", loan.loanID, loan.customerID , loan.amount, (loan.approved==0? "Pending" : (loan.approved==1? "Approved": "Rejected") ) , loan.EmployeeID);

            if (strlen(writeBuffer) == 0)
                strcpy(writeBuffer, tempBuffer); //first transaction so copy it
            else
                strcat(writeBuffer, tempBuffer);  // adding more transactions in it
        }

        close(loanFileDescriptor);

        if (strlen(writeBuffer) == 0)
        {
            write(connFD, "No loans are presently assigned on this employee!^", strlen("No loans are presently assigned on this employee!^"));
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
}

bool add_customer_account(int connFD) //to make new account 
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Account newAccount, prevAccount;

    int accountFileDescriptor = open(ACCOUNT_FILE, O_RDONLY);

    if (accountFileDescriptor == -1 && errno == ENOENT)
    {
        // Account file was never created
        newAccount.accountNumber = 0;
    }
    else
    {
        int offset = lseek(accountFileDescriptor, -sizeof(struct Account), SEEK_END);
        //applying lock
        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Account), getpid()};
        int lockingStatus = fcntl(accountFileDescriptor, F_SETLKW, &lock);

        readBytes = read(accountFileDescriptor, &prevAccount, sizeof(struct Account));
        //unlocking
        lock.l_type = F_UNLCK;
        fcntl(accountFileDescriptor, F_SETLK, &lock);
        close(accountFileDescriptor);

        newAccount.accountNumber = prevAccount.accountNumber + 1;
    }

    newAccount.isRegularAccount = 1;

    newAccount.owner = add_customer(connFD, newAccount.accountNumber);
    if(newAccount.owner==-1)
    {
        writeBytes = write(connFD, "It seems you've enter a wrong input!\nYou'll now be redirected to the main menu!^", strlen("It seems you've enter a wrong input!\nYou'll now be redirected to the main menu!^"));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }

    newAccount.active = true;
    newAccount.balance = 0;

    memset(newAccount.transactions, -1, MAX_TRANSACTIONS * sizeof(int));

    accountFileDescriptor = open(ACCOUNT_FILE, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    writeBytes = write(accountFileDescriptor, &newAccount, sizeof(struct Account));
    close(accountFileDescriptor);

    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "%s%d", "The newly created account's number is :", newAccount.accountNumber);
    strcat(writeBuffer, "\nRedirecting you to the main menu ...^");
    writeBytes = write(connFD, writeBuffer, sizeof(writeBuffer));
    readBytes = read(connFD, readBuffer, sizeof(read)); // Dummy read
    return true;
}

int add_customer(int connFD, int newAccountNumber)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];
    bzero(readBuffer, sizeof(readBuffer));
    bzero(writeBuffer, sizeof(writeBuffer));

    struct Customer newCustomer, previousCustomer;

    int customerFileDescriptor = open(CUSTOMER_FILE, O_RDONLY);
    if (customerFileDescriptor == -1 && errno == ENOENT)
    {
        // Customer file was never created
        newCustomer.id = 0;
    }
    else
    {
        int offset = lseek(customerFileDescriptor, -sizeof(struct Customer), SEEK_END);

        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};
        fcntl(customerFileDescriptor, F_SETLKW, &lock);

        readBytes = read(customerFileDescriptor, &previousCustomer, sizeof(struct Customer));

        lock.l_type = F_UNLCK;
        fcntl(customerFileDescriptor, F_SETLK, &lock);
        close(customerFileDescriptor);

        newCustomer.id = previousCustomer.id + 1;
    }

    sprintf(writeBuffer, "%s%s", "Enter the details for the customer\n", "What is the customer's name?");

    writeBytes = write(connFD, writeBuffer, sizeof(writeBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    strcpy(newCustomer.name, readBuffer);

    writeBytes = write(connFD, "What is the customer's gender? \nEnter M for male, F for female.", strlen("What is the customer's gender? \nEnter M for male, F for female."));

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));

    if (readBuffer[0] == 'M' || readBuffer[0] == 'F')
        newCustomer.gender = readBuffer[0];
    else
    {
        writeBytes = write(connFD, "It seems you've enter a wrong gender choice!\nYou'll now be redirected to the main menu!^", strlen("It seems you've enter a wrong gender choice!\nYou'll now be redirected to the main menu!^"));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return -1;
    }

    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "What is the customer's age?");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));

    int customerAge = atoi(readBuffer);
    if (customerAge == 0)
    {
        // Either client has sent age as 0 (which is invalid) or has entered a non-numeric string
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "You have entered an invalid age!\nNow, you'll be redirected to the main menu!^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return -1;
    }

    newCustomer.age = customerAge;
    newCustomer.account = newAccountNumber;

    strcpy(newCustomer.login, newCustomer.name);
    strcat(newCustomer.login, "-");
    sprintf(writeBuffer, "%d", newCustomer.id);
    strcat(newCustomer.login, writeBuffer);

    char hashedPassword[1000];
    strcpy(hashedPassword, crypt("customer123", SALT_BAE));
    strcpy(newCustomer.password, hashedPassword);

    customerFileDescriptor = open(CUSTOMER_FILE, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    writeBytes = write(customerFileDescriptor, &newCustomer, sizeof(newCustomer));
    close(customerFileDescriptor);

    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "%s%s-%d\n%s%s", "The login ID for the customer is : ", newCustomer.name, newCustomer.id, "The autogenerated password for the customer is : ", "customer123");
    strcat(writeBuffer, "^");

    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
    return newCustomer.id;
}

bool modify_customer_info(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Customer customer;

    int customerID;

    off_t offset;
    int lockingStatus;

    writeBytes = write(connFD, "Enter the ID of the customer who's information you want to edit", strlen("Enter the ID of the customer who's information you want to edit"));
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));

    customerID = atoi(readBuffer);

    int customerFileDescriptor = open(CUSTOMER_FILE, O_RDONLY);
    if (customerFileDescriptor == -1)
    {
        // Customer File doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "No customer file exists^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    
    offset = lseek(customerFileDescriptor, customerID * sizeof(struct Customer), SEEK_SET);
    if (errno == EINVAL)
    {
        // Customer record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "No customer could be found for the given ID^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    else if (offset == -1)
    {
        perror("Error while seeking to required customer record!");
        return false;
    }


    struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};
    // Locking record
    lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    readBytes = read(customerFileDescriptor, &customer, sizeof(struct Customer));
    // Unlocking record
    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLK, &lock);
    close(customerFileDescriptor);


    writeBytes = write(connFD, "Which information would you like to modify?\n1. Name 2. Age 3. Gender \nPress any other key to cancel", strlen("Which information would you like to modify?\n1. Name 2. Age 3. Gender \nPress any other key to cancel"));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));

    int choice = atoi(readBuffer);

    if (choice == 0)
    { // A non-numeric string was passed to atoi
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "You have entered an invalid number!\nYou'll now be redirected to the main menu!^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    switch (choice)
    {
    case 1:
        writeBytes = write(connFD, "What's the updated value for name?", strlen("What's the updated value for name?"));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        strcpy(customer.name, readBuffer);
        break;

    case 2:
        writeBytes = write(connFD, "What's the updated value for age?", strlen("What's the updated value for age?"));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        int updatedAge = atoi(readBuffer);
        if (updatedAge == 0)
        {
            // Either client has sent age as 0 (which is invalid) or has entered a non-numeric string
            bzero(writeBuffer, sizeof(writeBuffer));
            strcpy(writeBuffer, "You have entered an invalid number! \nYou'll now be redirected to the main menu!^");
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
            return false;
        }
        customer.age = updatedAge;
        break;

    case 3:
        writeBytes = write(connFD, "What's the updated value for gender?", strlen("What's the updated value for gender?"));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        customer.gender = readBuffer[0];
        break;

    default:
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "You have entered an invalid number! \nYou'll now be redirected to the main menu!^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }


    customerFileDescriptor = open(CUSTOMER_FILE, O_WRONLY);
    offset = lseek(customerFileDescriptor, customerID * sizeof(struct Customer), SEEK_SET);

    lock.l_type = F_WRLCK;
    lock.l_start = offset;
    lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    writeBytes = write(customerFileDescriptor, &customer, sizeof(struct Customer));
    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLKW, &lock);
    close(customerFileDescriptor);


    writeBytes = write(connFD, "The required modification was successfully made! \nYou'll now be redirected to the main menu!^", strlen("The required modification was successfully made! \nYou'll now be redirected to the main menu!^"));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

    return true;
}


#endif