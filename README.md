# R-Link
Contains 

# The R programming language
R is an open source programming language and software environment for statistical computing that is broadly used among statisticians, econometrists and data miners. R implements a wide variety of statistical techniques such as linear and nonlinear modeling, time-series analysis, classification and clustering. 

# R4AIMMS
R4AIMMS is an AIMMS system library that allows AIMMS to execute R user-defined code and provides data-passing facilities from AIMMS to the R runtime and vice versa. R4AIMMS extends the AIMMS experience with R’s universe of statistical analysis tools, ultimately allowing AIMMS users to write and use their own R scripts/libraries through AIMMS.

# Requirements
The R4AIMMS system library requires that the R programming language is already installed on the target machine along with the Rcpp R package. The Rcpp package requires R’s version to be 3.0.0 or higher and can be installed from within R by typing install.packages(“Rcpp”).

# R::executeScript Procedure
The library exposes the executeScript procedure through the R prefix. The procedure takes a scalar string as argument, representing the R script to be executed through AIMMS. The R script can contain any arbitrary R code, such as function calls, R object declarations, library installation/import statements and so on. In case of an R error, AIMMS will notify the user by raising this error. During R code execution through R::executeScript there is no visible output from the R output, thus the result to a call to R’s print function won’t be visible to the AIMMS user.



# Data passing between AIMMS and the R runtime environment
We provide aimms4r, an R library that facilitates data passing between AIMMS and the R runtime. Aimms4r contains two R functions, the aimms4r::GetData and the aimms4r::SetData. 

# Aimms4r::GetData
Aimms4r::GetData(scalar string: fully qualified identifier name)
Aimms4r::GetData accepts as argument a scalar string representing the fully qualified name of an AIMMS identifier and returns either an R data frame in case of a multidimensional identifier, or a scalar R object containing the value of the AIMMS scalar. A value can have integer, string, set element or double storage data type. By calling GetData with the appropriate identifier’s name, the user can expose the identifier’s data content to the R universe and therefore apply any operation freely. In case there is no identifier with the name specified by the function’s input argument, the aimms4r::GetData function call will fail resulting in an error being raised in AIMMS. 

# Data Frame Form
Consider the 4-dimensional Identifier MultiDim(i,j,k,l), with i an index to the set I, j an index to the set J, k an index to the set K and l and index to set L subsequently.

    Set setI {
        Index: i;
        InitialData: {
            {1..2}
        }
    }
    Set setJ {
        Index: j;
        InitialData: {
            {1..5}
        }
    }
    Set setK {
        Index: k;
        InitialData: {
            {1..10}
        }
    }
    Set setL {
        Index: l;
        InitialData: {
            {1..20}
        }
    }
    Parameter MultiDim {
        IndexDomain: (i,j,k,l);
    }
    
After calling aimms4r::GetData(“MultiDim”), AIMMS will return an R data frame holding the data of the identifier MultiDim. The data frame will contain 5 columns where the first 4 columns will have the name of the set of the indices (columns setI,setJ,setK,setL). The last column of the data frame will have the same name as the AIMMS Identifier itself (column MultiDim). In the R context (during the execution of the R string code passed to R::executeScript), the data frame will be available and its column-specific data will be available by using the standard R data frame element access expressions. 
Consider the following R code:

multiDim<-aimms4r::GetData(“MultiDim”);
/*Access the first column (setI) by index value*/
iColumn<-multiDim[1]
/*Access the second column (setJ) by column name*/
jColumn<-multiDim[‘setJ’]
/*Access the data column through the dollar sign notation*/
valueColumn<-multiDim$multiDim
    

# Aimms Storage Types and R Data Frame Column Data Types
Aimms storage types such as integer, double, set element and string are maintained after retrieving AIMMS data through a call to GetData. Sets that are subsets of the Integers set will be retrieved as integer Columns. Normal sets will be retrieved as string Columns. Similarly, the values of element parameters will be retrieved depending on their range storage type.

# Aimms4r::SetData
Aimms4r::SetData(r object, scalar string: fully qualified identifier name)
Aimms4r::SetData accepts two arguments, the first one is an R data frame or an R scalar value and the second one is a scalar string that represents the fully qualified name of an existing AIMMS identifier to which we want to assign the data of the first argument. In case of a mismatch between the argument data frame’s columns and the assigned identifier’s dimensions, an error will be raised and reported by AIMMS. Additionally, the function does not add new elements to the sets, in case it encounters a column element that does not exist in the corresponding set. In that case, an error will be raised by AIMMS, informing the user about that unknown element.

# Storage Type Conversions when assigning R data to AIMMS (functionality partially there)
The following conversion table holds when data is assigned to an identifier by using the aimms4r::SetData function. When we want to assign element data to an element identifier from R, the element value should be within range, for the assignment to be successful. Otherwise, it will fail and AIMMS will issue an error.
FROM/TO	Element	Integer	Double	String
Integer	OK if range=integer	OK	OK	FAIL
Double	OK if range=string	FAIL	OK	FAIL
String	OK if range =string	FAIL	FAIL	OK

 
# R Object Lifetime
Consider the following AIMMS statements where the R::executeScript procedure is used:
1.  ```R::executeScript("data<-aimms4r::GetData('Populations')");```
2.	```R::executeScript("populations<-data[3]");```
3.	```R::executeScript("populations <- populations * 100;");```
4.	```R::executeScript("data[3]<-populations");```
5.	```R::executeScript("aimms4r::SetData('Populations')");```

In line 1, we retrieve the data of the AIMMS identifier “Populations” and in lines 2-4, we perform some calculations on the third column of the data frame containing the data of the identifier. In line 5, we finally assign back the processed data to the identifier. Consider that the R data frame “data” (created in line 1) and the R list “populations” are maintained through the sequence of different calls to the R::executeScript. After the last statement, both R objects will still be visible in the R context, thus they will still allocate some memory. In case that the “Populations” identifier contains a big amount of data, both the “data” data frame and the “populations” list will still require big amounts of memory. The AIMMS developer should keep that in mind and administer the memory usage consumed by the R context. Thus, R object clean statements could be added to ensure that we are not using more memory than needed. In our case, we could issue the following statement (assuming that we do not need both R variables after assigning the data to the identifier in line 5):

6.	```R::executeScript("data<-NULL;populations<-NULL;");```


# AIMMS to R data passing and vice versa example
In this example, we will show how to use the R4AIMMS system library, how to execute R code through AIMMS and how to pass data along AIMMS to R and vice versa.

1.	Open AIMMS and create a new project named “RAimmsTest”.
2.	Add the system library “R4AIMMS” to the model.
3.	Create the Set “Amount” and specify its elements.
4.	Create the one-dimensional Identifier “Cosines(i)”.
5.	Create the procedure “fillCosines” and place the following two statements in its body attribute:
```
Cosines(i) := ord(i);
R::executeScript("data<-aimms4r::GetData('Cosines');"+
                 "data[2]<-cos(data[2]);" +
                 "aimms4r::SetData(data, 'Cosines');");
```    
The first statement initializes the content of the Cosines identifier with the ordinal of each element of the set “Amount”.
The second statement executes the R code which:
1.	Retrieves a data frame named ‘data’ containing Cosines’s data by calling aimms4r::GetData
2.	It accesses the second column of the Data Frame (the column that holds the actual data), applies the cosine function to each element of the column and assigns back the new data frame back to the data object
3.	It writes the data back to the AIMMS “Cosines” identifier by calling aimms4r::SetData.


# AIMMS4R package documentation
GetData
Usage
GetData(identifierName)
Arguments
identifierName  
An R Character variable with the name of the AIMMS identifier from which we want to retrieve data
Value
If the identifier is a multidimensional identifier, the function will return a data frame containing the identifier’s data. Depending on the identifier’s dimensions, the data frame will first contain columns representing the set elements over which the identifier has a non-zero value (row-based tuples) and an additional column representing the actual data. The columns representing the set elements can either have a string or integer data type depending on how their sets are declared in AIMMS. The value column can have either integer, double or string data type. In case the identifier is scalar, the function will return a scalar R object with the same data type as the scalar identifier’s storage data type. Thus, the returned R object could be of integer, double, element or string data type. 
Details
In case an unknown identifier’s name has been provided as argument, the function will raise an error.
See Also
aimms4r::SetData
Examples
##Assuming there is an AIMMS identifier with name “Populations” in the model
data<-aimms4r::GetData(“Populations”)

SetData
Usage
SetData(data, identifierName)
Arguments
data An R object containing data that, after the successful call of the function, will be assigned to the underlying AIMMS identifier. In case the identifier to which we want to assign the data has multiple dimensions, the R object needs to be a data frame with columns equal to the amount of assigned identifier’s dimensions plus one,  for the value column. The column elements should be contained in the corresponding AIMMS sets (to which the identifier indices are bound). The value column elements should have the same storage type as the identifier’s storage type. In case the AIMMS identifier is scalar, the R object also needs to be scalar and should have the same storage data type as the identifier.  
identifierName The AIMMS identifier in which we want to store the underlying data.
Details
The function will raise an error when:
•	an unknown identifier name has been provided as argument
•	the R data frame does not have the same number of columns plus one as the AIMMS identifier
•	the R object represents an AIMMS set element and that element is not contained in the range of the identifier 
Examples
##Consider the following AIMMS declarations:
“Set Cities {
        Index: c;
        InitialData: data{'Amsterdam','Haarlem','Utrecht'};
    }
   Parameter PopulationsInK {
        IndexDomain: (c);
        InitialData: 0;
    }
    ElementParameter CurrentCity {
        Range: Cities;
        InitialData: ‘Utrecht’;
    }
“
population<-data.frame(c(‘Amsterdam’,’Haarlem’),c(779,150 ))
## Set multidimensional identifier data
SetData(population,’PopulationsInK’)
## Set scalar set element
SetData(‘Amsterdam’,’CurrentCity’)



