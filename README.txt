// --------- READ ME begins --------- //

// Last Change Time: 20151123_1900

// Contents
//
//	source: a folder contains the source code, makefile and executables
//	
//	resource: a folder contains the resource images the source code needs
//	
//	output: a folder contains the output image after execute the executable
//
//	Report.pdf: a file includes the results images and analysis
//

// Instruction:
//
// 	(1)	To compile the file, please enter “make” (shell)
//		in the command line under the following path:
//		
//			your path to the folder / DVT_HW4_B01901014 / source
//
//	(2)	To execute the executable, please enter “./main” (shell)
//		in the command line under the following path:
//
//			your path to the folder / DVT_HW4_B01901014 / source
//	
//	(3)	To change the image you want to process with this program,
//		change the FILENAME variable in the 59th line of def.h:
//		
//			const string FILENAME = “filename”;
//
//	(4)	After you execute the executable, you can view the output
//		raw files in the following path:
//			
//			your path to the folder / DVT_HW4_B01901014 / output
//
//		There should be several files in total for a single photo chosen
//		to process in the former step. For example, if we pick 
//		“weather” for the FILENAME and -8~7 for search range, the program
// 		generates the following files.
//		
//		1. weather_W8.y: a fully recovered video sequence
//
//		2. weather_W8_rec.y: the resulting video sequence after MC is taken
//
//		3. weather_W8_res.y: the residue sequence of the input file
//
//		4. weather_W8_dif.y: the difference sequence of the input file
//
//		5. weather_W8.dat: a text file recording the psnr value of every frame
//
//		6. weather_W8.tim: a text file recording the computational time
//		   of every frame
//
//		* notice that W8 denotes the search range of [-8,7] and W16 [-16,15].
//		  Plus, fast denotes that the file is generated with fast search strategy.
// 		  You may change the search range in the 45th line of def.h to 16 or 32,
//		  corresponding to W8 and W16. Also, you can change to fast search strategy
// 		  by adding and ‘f’ before the “ME” in the 68th line of main.cpp.
//
//		The whole process will be shown, with PSNR, computational time and
// 		current progress of coding in percentage.
//
//	(5)	To remove the existed object files and executable, please enter
// 		“./make clean” (shell) in the command line under the following path:
//
//			your path to the folder / DVT_HW4_B01901014 / source
//	
//

// --------- READ ME ends --------- //
