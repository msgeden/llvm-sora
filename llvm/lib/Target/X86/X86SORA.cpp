#include "X86.h"
#include "X86InstrInfo.h"
#include "X86InstrBuilder.h"
#include "X86Subtarget.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"

using namespace llvm;

#define X86_SORA_PASS_NAME "Integrity Checks of Registers Spills Through MAC tags"

namespace {
class X86SORA : public MachineFunctionPass {
public:
    static char ID;
    X86SORA() : MachineFunctionPass(ID) {
        initializeX86SORAPass(*PassRegistry::getPassRegistry());
    }
    bool runOnMachineFunction(MachineFunction &MF) override;
    StringRef getPassName() const override { return X86_SORA_PASS_NAME; }
private:
    const TargetInstrInfo *TII; // Machine instruction info.
    const X86Subtarget *STI; // Architecture info.
    bool is64bit;
};
char X86SORA::ID = 0;
bool X86SORA::runOnMachineFunction(MachineFunction &MF) {
    TII = MF.getSubtarget().getInstrInfo();
    STI = &(MF.getSubtarget<X86Subtarget>());
    is64bit=STI->is64Bit();
    //Generates 128*bit key at the beginning of main function and stores it on XMM0
//    if (MF.getName()=="main"){
//        MachineBasicBlock &MBB=MF.front();
//        DebugLoc DL;
//        MachineBasicBlock::iterator MBBI=MBB.begin();
//        std::advance(MBBI,2);
//        //Register RAX=is64bit?X86::RAX:X86::EAX;
//        BuildMI(MBB,MBBI,DL,TII->get(X86::RDRAND64r)).addReg(X86::RAX);
//        BuildMI(MBB,MBBI,DL,TII->get(X86::MOV64toPQIrr)).addReg(X86::XMM0).addReg(X86::RAX);
//        BuildMI(MBB,MBBI,DL,TII->get(X86::PSLLDQri),X86::XMM0).addReg(X86::XMM0).addImm(8);
//        BuildMI(MBB,MBBI,DL,TII->get(X86::RDRAND64r)).addReg(X86::RAX);
//        BuildMI(MBB,MBBI,DL,TII->get(X86::MOV64toPQIrr)).addReg(X86::XMM0).addReg(X86::RAX);
//        BuildMI(MBB,MBBI,DL,TII->get(X86::ADD64ri8),X86::RSP).addReg(X86::RSP).addImm(0);
//        //BuildMI(MBB,MBBI,DL,TII->get(X86::MOV64rm)).;
//        //int EHRegSize = MFI.getObjectSize(FI);
//        addRegOffset(BuildMI(MBB, MBBI, DL, TII->get(X86::MOV32rm), X86::ESP),
//                     X86::EBP, true, -4);
//        //BuildMI(MBB, MBBI, DL, TII->get(X86::MOV64rm)).addReg(X86::RBP).addMemOperand(X86::RSP,true,8);
//         //            X86::EBP, true, -4)
//          //  .setMIFlag(MachineInstr::FrameSetup);
//    }
    for (auto &MBB : MF) {
        outs() << "Contents of MachineBasicBlock:\n";
        outs() << MBB << "\n";
        const BasicBlock *BB = MBB.getBasicBlock();
        outs() << "Contents of BasicBlock corresponding to MachineBasicBlock:\n";
        outs() << BB << "\n";
    }
    return false;
}
} // end of anonymous namespace
INITIALIZE_PASS(X86SORA, "x86-sora",
                X86_SORA_PASS_NAME,
                true, // is CFG only?
                true  // is analysis?
)
namespace llvm {
FunctionPass *createX86SORAPass() { return new X86SORA(); }
}
