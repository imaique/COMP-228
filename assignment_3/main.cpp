//
// Created by Michael on 2021-04-04.
//
/*

loop:
  fld  f2,4(r3)               // assembly-language program
  fld  f4,8(r5)               // to be simulated;
  fmul f6,f2,f4               // no pipelining of datapath
  fadd f8,f4,f6
  fst  f6,8(r5)               // assembly-language program
  fst  f8,4(r3)               // consists of one loop
  fld  f2,8(r7)
  fst  f2,8(r7)
  subi r9,r9,1
  bne  r9,r1,loop             // r1 is permanently 0


*/

#include <iostream>           // for std::cout

int main () {
    // all submissions must contain this prelude, or equivalent

    // register file                          // ten registers:
    int Reg[10];                              // five 'f_reg' & five 'r_reg'
    Reg[1] = 0; Reg[9] = 2;                   // initial r-register values
    Reg[3] = 303; Reg[5] = 505; Reg[7] = 707;

    // main memory
    int  Mem[800];
    // u = substract, s = store, b = branch
    char XMem[] = "llmasslsub";               // ten opcodes, and
    Mem[0] = 324; Mem[1] = 548; Mem[2] = 246; // their ten argument
    Mem[3] = 468; Mem[4] = 568; Mem[5] = 384; // lists
    Mem[6] = 728; Mem[7] = 728; Mem[8] = 991;
    Mem[9] = 910;

    Mem[307] = 41; Mem[513] = 23;             // initial memory values
    Mem[715] = 37;

    bool branch;                              // to leave ass. lang. loop
    std::cout << "\n";                        // blank line

    // loop over 'n' iterations of the loop body
    for( int iter = 0; iter < 4; iter++ ) {

        std::cout << "Iteration number " << iter + 1 << ".\n\n";

        // loop over ten instructions in the loop body
        for( int PC = 0; PC < 10; PC++ ) {
            std::cout << "\n";
            // f-box
            char head = XMem[PC];
            int  tail = Mem[PC];
            std::cout << "f: Fetched instruction: " << head << "|" << tail << ".\n";
            // end f-box

            // d-box
            char opc = head;                        // decompose instruction
            int arg3 = tail % 10;                   // into its opcode and
            tail = tail / 10;                       // its three arguments
            int arg2 = tail % 10;                   // inst = {opc,arg1,arg2,arg3}
            tail = tail / 10;
            int arg1 = tail;
            std::cout << "d: Set opc to '" << opc << "'.\n";
            // all code, or equivalent, above this line is mandatory

            int D_Out1, D_Out2, dreg, sval;         // 'branch' declared earlier
            switch( opc ) {
                case 's':
                    sval = Reg[arg2];                 // localize and latch
                    std::cout << "d: Set sval to the value in f" << arg2 << ", " << sval << ".\n";
                    D_Out1 = Reg[arg1];                 // localize and latch
                    std::cout << "d: Set D_Out1 to " << D_Out1 << ".\n";
                    D_Out2 = arg3;
                    std::cout << "d: Set D_Out2 to " << D_Out2 << ".\n";
                    break;

                case 'l':
                    D_Out1 = Reg[arg1];                 // localize and latch
                    std::cout << "d: Set D_Out1 to " << D_Out1 << ".\n";
                    D_Out2 = arg3;                 // localize and latch
                    std::cout << "d: Set D_Out2 to " << D_Out2 << ".\n";
                    dreg = arg2;
                    std::cout << "d: Set dreg to f" << dreg << ".\n";
                    break;
                case 'a': case 'm':
                    D_Out1 = Reg[arg1];                 // localize and latch
                    std::cout << "d: Set D_Out1 to " << D_Out1 << ".\n";
                    D_Out2 = Reg[arg2];                 // localize and latch
                    std::cout << "d: Set D_Out2 to " << D_Out2 << ".\n";
                    dreg = arg3;                        // record dest. register
                    std::cout << "d: Set dreg to f" << dreg << ".\n";
                    break;
                case 'u':
                    D_Out1 = Reg[arg1];
                    std::cout << "d: Set D_Out1 to " << D_Out1 << ".\n";
                    D_Out2 = arg3;                 // localize and latch
                    std::cout << "d: Set D_Out2 to " << D_Out2 << ".\n";
                    dreg = arg2;                        // record dest. register
                    std::cout << "d: Set dreg to r" << dreg << ".\n";
                    break;
                case 'b':
                    if(Reg[arg1]==Reg[arg2]){
                        branch=false;
                        std::cout << "d: Set branch to false.\n";
                    }else{
                        branch=true;
                        std::cout << "d: Set branch to true.\n";
                    }
                    break;

                default:
                    std::cout << "d: Did nothing.\n";
                    break;
            }

            // x-box
            int X_Out;
            switch( opc ) {
                case 's': case 'l': case 'a':
                    X_Out = D_Out1 + D_Out2;
                    std::cout << "x: Set X_Out to " << X_Out << ".\n";
                    break;
                case 'u':
                    X_Out = D_Out1 - D_Out2;
                    std::cout << "x: Set X_Out to " << X_Out << ".\n";
                    break;
                case 'm':
                    X_Out = D_Out1 * D_Out2;
                    std::cout << "x: Set X_Out to " << X_Out << ".\n";
                    break;

                    //...
                default:
                    std::cout << "x: Did nothing.\n";
                    break;
            }

            // m-box
            int M_Out;
            switch( opc ) {
                case 'l':
                    M_Out = Mem[X_Out];
                    std::cout << "m: Set M_Out to " << M_Out << ".\n";
                    break;
                    // case 'm': do nothing, case 'a': do nothing, case 'u':
                case 's':
                    Mem[X_Out] = sval;
                    std::cout << "m: Set Mem[" << X_Out <<"] to " << Mem[X_Out] << ".\n";
                    break;
                    //...
                default:
                    std::cout << "m: Did nothing.\n";
                    break;
            }

            // w-box
            switch( opc ){
                case 'l':
                    Reg[dreg] = M_Out;
                    std::cout << "w: Set f" << dreg <<" to " << Reg[dreg] << ".\n";
                    break;

                case 'm': case 'a':
                    Reg[dreg] = X_Out;
                    std::cout << "w: Set f" << dreg <<" to " << Reg[dreg] << ".\n";
                    break;
                case 'u':
                    Reg[dreg] = X_Out;
                    std::cout << "w: Set r" << dreg <<" to " << Reg[dreg] << ".\n";

                    //...
                default:
                    std::cout << "w: Did nothing.\n";
                    break;
            }

        } // end inner loop

        std::cout << "\n";

        //...
        if(!branch){
            std::cout << "Exit loop after " << iter  + 1 << " iterations.\n";
            std::cout << "\n";
            break;
        }

    } // end outer loop
    //...
    std::cout << "Main program terminates normally \n";
} // end program