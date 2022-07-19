
kernel:     file format elf64-littleriscv


Disassembly of section .text:

0000000080000000 <_entry>:
    80000000:	00006117          	auipc	sp,0x6
    80000004:	0c813103          	ld	sp,200(sp) # 800060c8 <_GLOBAL_OFFSET_TABLE_+0x10>
    80000008:	00001537          	lui	a0,0x1
    8000000c:	f14025f3          	csrr	a1,mhartid
    80000010:	00158593          	addi	a1,a1,1
    80000014:	02b50533          	mul	a0,a0,a1
    80000018:	00a10133          	add	sp,sp,a0
    8000001c:	351020ef          	jal	ra,80002b6c <start>

0000000080000020 <spin>:
    80000020:	0000006f          	j	80000020 <spin>
	...

0000000080001000 <__environmentCall__>:
.global __environmentCall__
.type _environmentCall__, @function
__environmentCall__:
    ecall
    80001000:	00000073          	ecall
    80001004:	00008067          	ret
	...

0000000080001010 <__supervisorTrap__>:
# void kernel::supervisorTrap()
.global __supervisorTrap__
.type __supervisorTrap__, @function
.align 4
__supervisorTrap__:
    csrw sscratch, t6
    80001010:	140f9073          	csrw	sscratch,t6
    ld t6, __runningThread
    80001014:	00005f97          	auipc	t6,0x5
    80001018:	13cfbf83          	ld	t6,316(t6) # 80006150 <__runningThread>
    .irp index 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30
        sd x\index, offsetTCBregisters + \index * 8 (t6)
    .endr
    8000101c:	001fbc23          	sd	ra,24(t6)
    80001020:	022fb023          	sd	sp,32(t6)
    80001024:	023fb423          	sd	gp,40(t6)
    80001028:	024fb823          	sd	tp,48(t6)
    8000102c:	025fbc23          	sd	t0,56(t6)
    80001030:	046fb023          	sd	t1,64(t6)
    80001034:	047fb423          	sd	t2,72(t6)
    80001038:	048fb823          	sd	s0,80(t6)
    8000103c:	049fbc23          	sd	s1,88(t6)
    80001040:	06afb023          	sd	a0,96(t6)
    80001044:	06bfb423          	sd	a1,104(t6)
    80001048:	06cfb823          	sd	a2,112(t6)
    8000104c:	06dfbc23          	sd	a3,120(t6)
    80001050:	08efb023          	sd	a4,128(t6)
    80001054:	08ffb423          	sd	a5,136(t6)
    80001058:	090fb823          	sd	a6,144(t6)
    8000105c:	091fbc23          	sd	a7,152(t6)
    80001060:	0b2fb023          	sd	s2,160(t6)
    80001064:	0b3fb423          	sd	s3,168(t6)
    80001068:	0b4fb823          	sd	s4,176(t6)
    8000106c:	0b5fbc23          	sd	s5,184(t6)
    80001070:	0d6fb023          	sd	s6,192(t6)
    80001074:	0d7fb423          	sd	s7,200(t6)
    80001078:	0d8fb823          	sd	s8,208(t6)
    8000107c:	0d9fbc23          	sd	s9,216(t6)
    80001080:	0fafb023          	sd	s10,224(t6)
    80001084:	0fbfb423          	sd	s11,232(t6)
    80001088:	0fcfb823          	sd	t3,240(t6)
    8000108c:	0fdfbc23          	sd	t4,248(t6)
    80001090:	11efb023          	sd	t5,256(t6)
    csrr t5, sscratch
    80001094:	14002f73          	csrr	t5,sscratch
    sd t5, offsetTCBregisters + 31 * 8(t6)
    80001098:	11efb423          	sd	t5,264(t6)
    csrr t5, sepc
    8000109c:	14102f73          	csrr	t5,sepc
    sd t5, offsetTCBpc(t6)
    800010a0:	01efb023          	sd	t5,0(t6)
    csrr t5, sstatus
    800010a4:	10002f73          	csrr	t5,sstatus
    sd t5, offsetTCBsstatus(t6)
    800010a8:	01efb423          	sd	t5,8(t6)
    ld sp, systemStackTopAddress
    800010ac:	00005117          	auipc	sp,0x5
    800010b0:	00413103          	ld	sp,4(sp) # 800060b0 <systemStackTopAddress>

    call __supervisorTrapHandle__
    800010b4:	438000ef          	jal	ra,800014ec <__supervisorTrapHandle__>

    ld t6, __runningThread
    800010b8:	00005f97          	auipc	t6,0x5
    800010bc:	098fbf83          	ld	t6,152(t6) # 80006150 <__runningThread>
    ld t5, offsetTCBpc(t6)
    800010c0:	000fbf03          	ld	t5,0(t6)
    csrw sepc, t5
    800010c4:	141f1073          	csrw	sepc,t5
    ld t5, offsetTCBsstatus(t6)
    800010c8:	008fbf03          	ld	t5,8(t6)
    csrw sstatus, t5
    800010cc:	100f1073          	csrw	sstatus,t5
    .irp index 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
        ld x\index, offsetTCBregisters + \index * 8 (t6)
    .endr
    800010d0:	018fb083          	ld	ra,24(t6)
    800010d4:	020fb103          	ld	sp,32(t6)
    800010d8:	028fb183          	ld	gp,40(t6)
    800010dc:	030fb203          	ld	tp,48(t6)
    800010e0:	038fb283          	ld	t0,56(t6)
    800010e4:	040fb303          	ld	t1,64(t6)
    800010e8:	048fb383          	ld	t2,72(t6)
    800010ec:	050fb403          	ld	s0,80(t6)
    800010f0:	058fb483          	ld	s1,88(t6)
    800010f4:	060fb503          	ld	a0,96(t6)
    800010f8:	068fb583          	ld	a1,104(t6)
    800010fc:	070fb603          	ld	a2,112(t6)
    80001100:	078fb683          	ld	a3,120(t6)
    80001104:	080fb703          	ld	a4,128(t6)
    80001108:	088fb783          	ld	a5,136(t6)
    8000110c:	090fb803          	ld	a6,144(t6)
    80001110:	098fb883          	ld	a7,152(t6)
    80001114:	0a0fb903          	ld	s2,160(t6)
    80001118:	0a8fb983          	ld	s3,168(t6)
    8000111c:	0b0fba03          	ld	s4,176(t6)
    80001120:	0b8fba83          	ld	s5,184(t6)
    80001124:	0c0fbb03          	ld	s6,192(t6)
    80001128:	0c8fbb83          	ld	s7,200(t6)
    8000112c:	0d0fbc03          	ld	s8,208(t6)
    80001130:	0d8fbc83          	ld	s9,216(t6)
    80001134:	0e0fbd03          	ld	s10,224(t6)
    80001138:	0e8fbd83          	ld	s11,232(t6)
    8000113c:	0f0fbe03          	ld	t3,240(t6)
    80001140:	0f8fbe83          	ld	t4,248(t6)
    80001144:	100fbf03          	ld	t5,256(t6)
    80001148:	108fbf83          	ld	t6,264(t6)

    sret
    8000114c:	10200073          	sret

0000000080001150 <_Z9mem_allocm>:

namespace SystemCalls = kernel::SystemCalls;
using CallType = SystemCalls::CallType;
using kernel::MemoryAllocator;

void* mem_alloc(size_t size){
    80001150:	ff010113          	addi	sp,sp,-16
    80001154:	00113423          	sd	ra,8(sp)
    80001158:	00813023          	sd	s0,0(sp)
    8000115c:	01010413          	addi	s0,sp,16
    auto blockCount = MemoryAllocator::byteSizeToBlockCount(size);
    80001160:	00001097          	auipc	ra,0x1
    80001164:	2a4080e7          	jalr	676(ra) # 80002404 <_ZN6kernel15MemoryAllocator20byteSizeToBlockCountEm>
    WRITE_TO_REGISTER(a1, blockCount);
    80001168:	00050593          	mv	a1,a0
    environmentCall(CallType::MemoryAllocate);
    8000116c:	00100513          	li	a0,1
    80001170:	00000097          	auipc	ra,0x0
    80001174:	e90080e7          	jalr	-368(ra) # 80001000 <__environmentCall__>
    RETURN_AS(void*);
    80001178:	00050513          	mv	a0,a0
}
    8000117c:	00813083          	ld	ra,8(sp)
    80001180:	00013403          	ld	s0,0(sp)
    80001184:	01010113          	addi	sp,sp,16
    80001188:	00008067          	ret

000000008000118c <_Z8mem_freePv>:

int mem_free(void* address) {
    8000118c:	ff010113          	addi	sp,sp,-16
    80001190:	00113423          	sd	ra,8(sp)
    80001194:	00813023          	sd	s0,0(sp)
    80001198:	01010413          	addi	s0,sp,16
    WRITE_TO_REGISTER(a1, address);
    8000119c:	00050593          	mv	a1,a0
    environmentCall(CallType::MemoryFree);
    800011a0:	00200513          	li	a0,2
    800011a4:	00000097          	auipc	ra,0x0
    800011a8:	e5c080e7          	jalr	-420(ra) # 80001000 <__environmentCall__>
    RETURN_AS(int);
    800011ac:	00050513          	mv	a0,a0
}
    800011b0:	0005051b          	sext.w	a0,a0
    800011b4:	00813083          	ld	ra,8(sp)
    800011b8:	00013403          	ld	s0,0(sp)
    800011bc:	01010113          	addi	sp,sp,16
    800011c0:	00008067          	ret

00000000800011c4 <_Z13thread_createPP7_threadPFvPvES2_>:

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg) {
    800011c4:	fd010113          	addi	sp,sp,-48
    800011c8:	02113423          	sd	ra,40(sp)
    800011cc:	02813023          	sd	s0,32(sp)
    800011d0:	00913c23          	sd	s1,24(sp)
    800011d4:	01213823          	sd	s2,16(sp)
    800011d8:	01313423          	sd	s3,8(sp)
    800011dc:	03010413          	addi	s0,sp,48
    800011e0:	00050493          	mv	s1,a0
    800011e4:	00058913          	mv	s2,a1
    800011e8:	00060993          	mv	s3,a2
    auto stack_space = mem_alloc(DEFAULT_STACK_SIZE);
    800011ec:	00001537          	lui	a0,0x1
    800011f0:	00000097          	auipc	ra,0x0
    800011f4:	f60080e7          	jalr	-160(ra) # 80001150 <_Z9mem_allocm>
    if (stack_space == nullptr) return -0x02;
    800011f8:	04050263          	beqz	a0,8000123c <_Z13thread_createPP7_threadPFvPvES2_+0x78>

    WRITE_TO_REGISTER(a4, stack_space);
    800011fc:	00050713          	mv	a4,a0
    WRITE_TO_REGISTER(a3, arg);
    80001200:	00098693          	mv	a3,s3
    WRITE_TO_REGISTER(a2, start_routine);
    80001204:	00090613          	mv	a2,s2
    WRITE_TO_REGISTER(a1, handle);
    80001208:	00048593          	mv	a1,s1

    environmentCall(CallType::ThreadCreate);
    8000120c:	01100513          	li	a0,17
    80001210:	00000097          	auipc	ra,0x0
    80001214:	df0080e7          	jalr	-528(ra) # 80001000 <__environmentCall__>

    RETURN_AS(int);
    80001218:	00050513          	mv	a0,a0
    8000121c:	0005051b          	sext.w	a0,a0
}
    80001220:	02813083          	ld	ra,40(sp)
    80001224:	02013403          	ld	s0,32(sp)
    80001228:	01813483          	ld	s1,24(sp)
    8000122c:	01013903          	ld	s2,16(sp)
    80001230:	00813983          	ld	s3,8(sp)
    80001234:	03010113          	addi	sp,sp,48
    80001238:	00008067          	ret
    if (stack_space == nullptr) return -0x02;
    8000123c:	ffe00513          	li	a0,-2
    80001240:	fe1ff06f          	j	80001220 <_Z13thread_createPP7_threadPFvPvES2_+0x5c>

0000000080001244 <_Z11thread_exitv>:


int thread_exit() {
    80001244:	ff010113          	addi	sp,sp,-16
    80001248:	00113423          	sd	ra,8(sp)
    8000124c:	00813023          	sd	s0,0(sp)
    80001250:	01010413          	addi	s0,sp,16
    environmentCall(CallType::ThreadExit);
    80001254:	01200513          	li	a0,18
    80001258:	00000097          	auipc	ra,0x0
    8000125c:	da8080e7          	jalr	-600(ra) # 80001000 <__environmentCall__>
    RETURN_AS(int);
    80001260:	00050513          	mv	a0,a0
}
    80001264:	0005051b          	sext.w	a0,a0
    80001268:	00813083          	ld	ra,8(sp)
    8000126c:	00013403          	ld	s0,0(sp)
    80001270:	01010113          	addi	sp,sp,16
    80001274:	00008067          	ret

0000000080001278 <_Z15thread_dispatchv>:

void thread_dispatch() {
    80001278:	ff010113          	addi	sp,sp,-16
    8000127c:	00113423          	sd	ra,8(sp)
    80001280:	00813023          	sd	s0,0(sp)
    80001284:	01010413          	addi	s0,sp,16
   environmentCall(CallType::ThreadDispatch);
    80001288:	01300513          	li	a0,19
    8000128c:	00000097          	auipc	ra,0x0
    80001290:	d74080e7          	jalr	-652(ra) # 80001000 <__environmentCall__>
}
    80001294:	00813083          	ld	ra,8(sp)
    80001298:	00013403          	ld	s0,0(sp)
    8000129c:	01010113          	addi	sp,sp,16
    800012a0:	00008067          	ret

00000000800012a4 <_Z8sem_openPP4_semj>:

int sem_open(sem_t* handle, unsigned init) {
    800012a4:	ff010113          	addi	sp,sp,-16
    800012a8:	00113423          	sd	ra,8(sp)
    800012ac:	00813023          	sd	s0,0(sp)
    800012b0:	01010413          	addi	s0,sp,16
    WRITE_TO_REGISTER(a2, init);
    800012b4:	00058613          	mv	a2,a1
    WRITE_TO_REGISTER(a1, handle);
    800012b8:	00050593          	mv	a1,a0
    environmentCall(CallType::SemaphoreOpen);
    800012bc:	02100513          	li	a0,33
    800012c0:	00000097          	auipc	ra,0x0
    800012c4:	d40080e7          	jalr	-704(ra) # 80001000 <__environmentCall__>
    RETURN_AS(int);
    800012c8:	00050513          	mv	a0,a0
}
    800012cc:	0005051b          	sext.w	a0,a0
    800012d0:	00813083          	ld	ra,8(sp)
    800012d4:	00013403          	ld	s0,0(sp)
    800012d8:	01010113          	addi	sp,sp,16
    800012dc:	00008067          	ret

00000000800012e0 <_Z9sem_closeP4_sem>:

int sem_close(sem_t handle) {
    800012e0:	ff010113          	addi	sp,sp,-16
    800012e4:	00113423          	sd	ra,8(sp)
    800012e8:	00813023          	sd	s0,0(sp)
    800012ec:	01010413          	addi	s0,sp,16
    WRITE_TO_REGISTER(a1, handle);
    800012f0:	00050593          	mv	a1,a0
    environmentCall(CallType::SemaphoreClose);
    800012f4:	02200513          	li	a0,34
    800012f8:	00000097          	auipc	ra,0x0
    800012fc:	d08080e7          	jalr	-760(ra) # 80001000 <__environmentCall__>
    RETURN_AS(int);
    80001300:	00050513          	mv	a0,a0
}
    80001304:	0005051b          	sext.w	a0,a0
    80001308:	00813083          	ld	ra,8(sp)
    8000130c:	00013403          	ld	s0,0(sp)
    80001310:	01010113          	addi	sp,sp,16
    80001314:	00008067          	ret

0000000080001318 <_Z8sem_waitP4_sem>:

int sem_wait(sem_t id) {
    80001318:	ff010113          	addi	sp,sp,-16
    8000131c:	00113423          	sd	ra,8(sp)
    80001320:	00813023          	sd	s0,0(sp)
    80001324:	01010413          	addi	s0,sp,16
    WRITE_TO_REGISTER(a1, id);
    80001328:	00050593          	mv	a1,a0
    environmentCall(CallType::SemaphoreWait);
    8000132c:	02300513          	li	a0,35
    80001330:	00000097          	auipc	ra,0x0
    80001334:	cd0080e7          	jalr	-816(ra) # 80001000 <__environmentCall__>
    RETURN_AS(int);
    80001338:	00050513          	mv	a0,a0
}
    8000133c:	0005051b          	sext.w	a0,a0
    80001340:	00813083          	ld	ra,8(sp)
    80001344:	00013403          	ld	s0,0(sp)
    80001348:	01010113          	addi	sp,sp,16
    8000134c:	00008067          	ret

0000000080001350 <_Z10sem_signalP4_sem>:

int sem_signal(sem_t id) {
    80001350:	ff010113          	addi	sp,sp,-16
    80001354:	00113423          	sd	ra,8(sp)
    80001358:	00813023          	sd	s0,0(sp)
    8000135c:	01010413          	addi	s0,sp,16
    WRITE_TO_REGISTER(a1, id);
    80001360:	00050593          	mv	a1,a0
    environmentCall(CallType::SemaphoreSignal);
    80001364:	02400513          	li	a0,36
    80001368:	00000097          	auipc	ra,0x0
    8000136c:	c98080e7          	jalr	-872(ra) # 80001000 <__environmentCall__>
    RETURN_AS(int);
    80001370:	00050513          	mv	a0,a0
    80001374:	0005051b          	sext.w	a0,a0
    80001378:	00813083          	ld	ra,8(sp)
    8000137c:	00013403          	ld	s0,0(sp)
    80001380:	01010113          	addi	sp,sp,16
    80001384:	00008067          	ret

0000000080001388 <_ZN6kernel11TrapHandler11incrementPCEv>:
#include "../../h/kernel/TrapHandlers.h"
#include "../../h/kernel/RegisterUtils.h"
#include "../../h/kernel/TCB.h"
#include "../../h/kernel/ConsoleUtils.h"

void kernel::TrapHandlers::incrementPC(){
    80001388:	ff010113          	addi	sp,sp,-16
    8000138c:	00113423          	sd	ra,8(sp)
    80001390:	00813023          	sd	s0,0(sp)
    80001394:	01010413          	addi	s0,sp,16
    auto runningThread = TCB::getRunningThread();
    80001398:	00001097          	auipc	ra,0x1
    8000139c:	c5c080e7          	jalr	-932(ra) # 80001ff4 <_ZN6kernel3TCB16getRunningThreadEv>
        TCB& operator=(TCB const&)=delete;
        ~TCB();

        uint64 getThreadId() const { return id; }
        Registers& getRegisters() { return context.registers; }
        uint64* getPC() const { return context.pc; }
    800013a0:	00053783          	ld	a5,0(a0) # 1000 <offsetTCBregisters+0xff0>
    auto pc = (uint64) runningThread->getPC();
    runningThread->setPC((uint64*)(pc + 4));
    800013a4:	00478793          	addi	a5,a5,4
        void setPC(uint64* value) { context.pc = value; }
    800013a8:	00f53023          	sd	a5,0(a0)
}
    800013ac:	00813083          	ld	ra,8(sp)
    800013b0:	00013403          	ld	s0,0(sp)
    800013b4:	01010113          	addi	sp,sp,16
    800013b8:	00008067          	ret

00000000800013bc <_ZN6kernel11TrapHandler22instructionErrorHandleEv>:

void kernel::TrapHandlers::instructionErrorHandle() {
    800013bc:	ff010113          	addi	sp,sp,-16
    800013c0:	00113423          	sd	ra,8(sp)
    800013c4:	00813023          	sd	s0,0(sp)
    800013c8:	01010413          	addi	s0,sp,16
    incrementPC();
    800013cc:	00000097          	auipc	ra,0x0
    800013d0:	fbc080e7          	jalr	-68(ra) # 80001388 <_ZN6kernel11TrapHandler11incrementPCEv>
    uint64 temp;
    asm volatile("csrr %0, scause":"=r"(temp));
    800013d4:	142025f3          	csrr	a1,scause
    printReg("scause",temp);
    800013d8:	00004517          	auipc	a0,0x4
    800013dc:	c4850513          	addi	a0,a0,-952 # 80005020 <CONSOLE_STATUS+0x10>
    800013e0:	00001097          	auipc	ra,0x1
    800013e4:	a68080e7          	jalr	-1432(ra) # 80001e48 <_Z8printRegPKcm>
    asm volatile("csrr  %0, sepc":"=r"(temp));
    800013e8:	141025f3          	csrr	a1,sepc
    printReg("sepc",temp);
    800013ec:	00004517          	auipc	a0,0x4
    800013f0:	c3c50513          	addi	a0,a0,-964 # 80005028 <CONSOLE_STATUS+0x18>
    800013f4:	00001097          	auipc	ra,0x1
    800013f8:	a54080e7          	jalr	-1452(ra) # 80001e48 <_Z8printRegPKcm>
    asm volatile("csrr %0, stval":"=r"(temp));
    800013fc:	143025f3          	csrr	a1,stval
    printReg("stval",temp);
    80001400:	00004517          	auipc	a0,0x4
    80001404:	c3050513          	addi	a0,a0,-976 # 80005030 <CONSOLE_STATUS+0x20>
    80001408:	00001097          	auipc	ra,0x1
    8000140c:	a40080e7          	jalr	-1472(ra) # 80001e48 <_Z8printRegPKcm>
}
    80001410:	00813083          	ld	ra,8(sp)
    80001414:	00013403          	ld	s0,0(sp)
    80001418:	01010113          	addi	sp,sp,16
    8000141c:	00008067          	ret

0000000080001420 <_ZN6kernel11TrapHandler16systemCallHandleEv>:

void kernel::TrapHandlers::systemCallHandle() {
    80001420:	fe010113          	addi	sp,sp,-32
    80001424:	00113c23          	sd	ra,24(sp)
    80001428:	00813823          	sd	s0,16(sp)
    8000142c:	00913423          	sd	s1,8(sp)
    80001430:	02010413          	addi	s0,sp,32
    using namespace SystemCalls;
    auto runningThread = TCB::getRunningThread();
    80001434:	00001097          	auipc	ra,0x1
    80001438:	bc0080e7          	jalr	-1088(ra) # 80001ff4 <_ZN6kernel3TCB16getRunningThreadEv>
    8000143c:	00050493          	mv	s1,a0
    incrementPC();
    80001440:	00000097          	auipc	ra,0x0
    80001444:	f48080e7          	jalr	-184(ra) # 80001388 <_ZN6kernel11TrapHandler11incrementPCEv>

    auto type = (CallType) runningThread->getRegisters().a0;
    80001448:	0604a783          	lw	a5,96(s1)

    switch (type) {
    8000144c:	02400713          	li	a4,36
    80001450:	02f76463          	bltu	a4,a5,80001478 <_ZN6kernel11TrapHandler16systemCallHandleEv+0x58>
    80001454:	00279793          	slli	a5,a5,0x2
    80001458:	00004717          	auipc	a4,0x4
    8000145c:	be070713          	addi	a4,a4,-1056 # 80005038 <CONSOLE_STATUS+0x28>
    80001460:	00e787b3          	add	a5,a5,a4
    80001464:	0007a783          	lw	a5,0(a5)
    80001468:	00e787b3          	add	a5,a5,a4
    8000146c:	00078067          	jr	a5
        case CallType::MemoryAllocate:
            return mem_alloc();
    80001470:	00000097          	auipc	ra,0x0
    80001474:	0e4080e7          	jalr	228(ra) # 80001554 <_ZN6kernel11SystemCalls9mem_allocEv>
        case CallType::GetChar:
            break;
        case CallType::PutChar:
            break;
    }
}
    80001478:	01813083          	ld	ra,24(sp)
    8000147c:	01013403          	ld	s0,16(sp)
    80001480:	00813483          	ld	s1,8(sp)
    80001484:	02010113          	addi	sp,sp,32
    80001488:	00008067          	ret
            return mem_free();
    8000148c:	00000097          	auipc	ra,0x0
    80001490:	120080e7          	jalr	288(ra) # 800015ac <_ZN6kernel11SystemCalls8mem_freeEv>
    80001494:	fe5ff06f          	j	80001478 <_ZN6kernel11TrapHandler16systemCallHandleEv+0x58>
            return thread_create();
    80001498:	00000097          	auipc	ra,0x0
    8000149c:	16c080e7          	jalr	364(ra) # 80001604 <_ZN6kernel11SystemCalls13thread_createEv>
    800014a0:	fd9ff06f          	j	80001478 <_ZN6kernel11TrapHandler16systemCallHandleEv+0x58>
            return thread_exit();
    800014a4:	00000097          	auipc	ra,0x0
    800014a8:	23c080e7          	jalr	572(ra) # 800016e0 <_ZN6kernel11SystemCalls11thread_exitEv>
    800014ac:	fcdff06f          	j	80001478 <_ZN6kernel11TrapHandler16systemCallHandleEv+0x58>
            return TCB::dispatch();
    800014b0:	00001097          	auipc	ra,0x1
    800014b4:	ac4080e7          	jalr	-1340(ra) # 80001f74 <_ZN6kernel3TCB8dispatchEv>
    800014b8:	fc1ff06f          	j	80001478 <_ZN6kernel11TrapHandler16systemCallHandleEv+0x58>
            return sem_open();
    800014bc:	00000097          	auipc	ra,0x0
    800014c0:	298080e7          	jalr	664(ra) # 80001754 <_ZN6kernel11SystemCalls8sem_openEv>
    800014c4:	fb5ff06f          	j	80001478 <_ZN6kernel11TrapHandler16systemCallHandleEv+0x58>
            return sem_close();
    800014c8:	00000097          	auipc	ra,0x0
    800014cc:	334080e7          	jalr	820(ra) # 800017fc <_ZN6kernel11SystemCalls9sem_closeEv>
    800014d0:	fa9ff06f          	j	80001478 <_ZN6kernel11TrapHandler16systemCallHandleEv+0x58>
            return sem_wait();
    800014d4:	00000097          	auipc	ra,0x0
    800014d8:	394080e7          	jalr	916(ra) # 80001868 <_ZN6kernel11SystemCalls8sem_waitEv>
    800014dc:	f9dff06f          	j	80001478 <_ZN6kernel11TrapHandler16systemCallHandleEv+0x58>
            return sem_signal();
    800014e0:	00000097          	auipc	ra,0x0
    800014e4:	3d4080e7          	jalr	980(ra) # 800018b4 <_ZN6kernel11SystemCalls10sem_signalEv>
    800014e8:	f91ff06f          	j	80001478 <_ZN6kernel11TrapHandler16systemCallHandleEv+0x58>

00000000800014ec <__supervisorTrapHandle__>:
void kernel::TrapHandlers::supervisorTrapHandle() {
    using TrapType = TrapHandlers::TrapType;

    TrapType trapCause;
    READ_FROM_SYS_REGISTER(scause, trapCause);
    800014ec:	142027f3          	csrr	a5,scause

    switch(trapCause) {
    800014f0:	00900713          	li	a4,9
    800014f4:	04f76e63          	bltu	a4,a5,80001550 <__supervisorTrapHandle__+0x64>
void kernel::TrapHandlers::supervisorTrapHandle() {
    800014f8:	ff010113          	addi	sp,sp,-16
    800014fc:	00113423          	sd	ra,8(sp)
    80001500:	00813023          	sd	s0,0(sp)
    80001504:	01010413          	addi	s0,sp,16
    switch(trapCause) {
    80001508:	00800713          	li	a4,8
    8000150c:	02e7f663          	bgeu	a5,a4,80001538 <__supervisorTrapHandle__+0x4c>
    80001510:	00500713          	li	a4,5
    80001514:	02e78863          	beq	a5,a4,80001544 <__supervisorTrapHandle__+0x58>
    80001518:	00700713          	li	a4,7
    8000151c:	02e78463          	beq	a5,a4,80001544 <__supervisorTrapHandle__+0x58>
    80001520:	00200713          	li	a4,2
    80001524:	02e78063          	beq	a5,a4,80001544 <__supervisorTrapHandle__+0x58>
        case TrapType::IllegalWriteAddress:
            return instructionErrorHandle();
        default:
            break;
    }
}
    80001528:	00813083          	ld	ra,8(sp)
    8000152c:	00013403          	ld	s0,0(sp)
    80001530:	01010113          	addi	sp,sp,16
    80001534:	00008067          	ret
            return systemCallHandle();
    80001538:	00000097          	auipc	ra,0x0
    8000153c:	ee8080e7          	jalr	-280(ra) # 80001420 <_ZN6kernel11TrapHandler16systemCallHandleEv>
    80001540:	fe9ff06f          	j	80001528 <__supervisorTrapHandle__+0x3c>
            return instructionErrorHandle();
    80001544:	00000097          	auipc	ra,0x0
    80001548:	e78080e7          	jalr	-392(ra) # 800013bc <_ZN6kernel11TrapHandler22instructionErrorHandleEv>
    8000154c:	fddff06f          	j	80001528 <__supervisorTrapHandle__+0x3c>
    80001550:	00008067          	ret

0000000080001554 <_ZN6kernel11SystemCalls9mem_allocEv>:
            registers.a0 = value;   \
            return;                 \
        }                           \
    }while(0)                       \

void kernel::SystemCalls::mem_alloc() {
    80001554:	fe010113          	addi	sp,sp,-32
    80001558:	00113c23          	sd	ra,24(sp)
    8000155c:	00813823          	sd	s0,16(sp)
    80001560:	00913423          	sd	s1,8(sp)
    80001564:	01213023          	sd	s2,0(sp)
    80001568:	02010413          	addi	s0,sp,32
    auto &registers = TCB::getRunningThread()->getRegisters();
    8000156c:	00001097          	auipc	ra,0x1
    80001570:	a88080e7          	jalr	-1400(ra) # 80001ff4 <_ZN6kernel3TCB16getRunningThreadEv>
    80001574:	00050493          	mv	s1,a0
    size_t blockCount = registers.a1;
    80001578:	06853903          	ld	s2,104(a0)
    auto memory = MemoryAllocator::getInstance().allocateBlocks(blockCount);
    8000157c:	00001097          	auipc	ra,0x1
    80001580:	e2c080e7          	jalr	-468(ra) # 800023a8 <_ZN6kernel15MemoryAllocator11getInstanceEv>
    80001584:	00090593          	mv	a1,s2
    80001588:	00001097          	auipc	ra,0x1
    8000158c:	e9c080e7          	jalr	-356(ra) # 80002424 <_ZN6kernel15MemoryAllocator14allocateBlocksEm>
    registers.a0 = (uint64) memory;
    80001590:	06a4b023          	sd	a0,96(s1)
}
    80001594:	01813083          	ld	ra,24(sp)
    80001598:	01013403          	ld	s0,16(sp)
    8000159c:	00813483          	ld	s1,8(sp)
    800015a0:	00013903          	ld	s2,0(sp)
    800015a4:	02010113          	addi	sp,sp,32
    800015a8:	00008067          	ret

00000000800015ac <_ZN6kernel11SystemCalls8mem_freeEv>:

void kernel::SystemCalls::mem_free() {
    800015ac:	fe010113          	addi	sp,sp,-32
    800015b0:	00113c23          	sd	ra,24(sp)
    800015b4:	00813823          	sd	s0,16(sp)
    800015b8:	00913423          	sd	s1,8(sp)
    800015bc:	01213023          	sd	s2,0(sp)
    800015c0:	02010413          	addi	s0,sp,32
    auto &registers = TCB::getRunningThread()->getRegisters();
    800015c4:	00001097          	auipc	ra,0x1
    800015c8:	a30080e7          	jalr	-1488(ra) # 80001ff4 <_ZN6kernel3TCB16getRunningThreadEv>
    800015cc:	00050493          	mv	s1,a0
    auto memory = (void *) registers.a1;
    800015d0:	06853903          	ld	s2,104(a0)
    int code = MemoryAllocator::getInstance().deallocateBlocks(memory);
    800015d4:	00001097          	auipc	ra,0x1
    800015d8:	dd4080e7          	jalr	-556(ra) # 800023a8 <_ZN6kernel15MemoryAllocator11getInstanceEv>
    800015dc:	00090593          	mv	a1,s2
    800015e0:	00001097          	auipc	ra,0x1
    800015e4:	f24080e7          	jalr	-220(ra) # 80002504 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv>
    registers.a0 = code;
    800015e8:	06a4b023          	sd	a0,96(s1)
}
    800015ec:	01813083          	ld	ra,24(sp)
    800015f0:	01013403          	ld	s0,16(sp)
    800015f4:	00813483          	ld	s1,8(sp)
    800015f8:	00013903          	ld	s2,0(sp)
    800015fc:	02010113          	addi	sp,sp,32
    80001600:	00008067          	ret

0000000080001604 <_ZN6kernel11SystemCalls13thread_createEv>:

void kernel::SystemCalls::thread_create() {
    80001604:	fc010113          	addi	sp,sp,-64
    80001608:	02113c23          	sd	ra,56(sp)
    8000160c:	02813823          	sd	s0,48(sp)
    80001610:	02913423          	sd	s1,40(sp)
    80001614:	03213023          	sd	s2,32(sp)
    80001618:	01313c23          	sd	s3,24(sp)
    8000161c:	01413823          	sd	s4,16(sp)
    80001620:	01513423          	sd	s5,8(sp)
    80001624:	01613023          	sd	s6,0(sp)
    80001628:	04010413          	addi	s0,sp,64
    auto &registers = TCB::getRunningThread()->getRegisters();
    8000162c:	00001097          	auipc	ra,0x1
    80001630:	9c8080e7          	jalr	-1592(ra) # 80001ff4 <_ZN6kernel3TCB16getRunningThreadEv>
    80001634:	00050493          	mv	s1,a0

    auto handle = (thread_t *) registers.a1;
    80001638:	06853983          	ld	s3,104(a0)
    auto task = (TCB::ThreadTask) registers.a2;
    8000163c:	07053a03          	ld	s4,112(a0)
    auto argument = (void *) registers.a3;
    80001640:	07853a83          	ld	s5,120(a0)
    auto stack = (void *) registers.a4;
    80001644:	08053b03          	ld	s6,128(a0)
    auto thread = new TCB(task, argument, stack);
    80001648:	14800513          	li	a0,328
    8000164c:	00001097          	auipc	ra,0x1
    80001650:	888080e7          	jalr	-1912(ra) # 80001ed4 <_ZN6kernel3TCBnwEm>
    80001654:	00050913          	mv	s2,a0
    80001658:	00050c63          	beqz	a0,80001670 <_ZN6kernel11SystemCalls13thread_createEv+0x6c>
    8000165c:	000b0693          	mv	a3,s6
    80001660:	000a8613          	mv	a2,s5
    80001664:	000a0593          	mv	a1,s4
    80001668:	00001097          	auipc	ra,0x1
    8000166c:	c80080e7          	jalr	-896(ra) # 800022e8 <_ZN6kernel3TCBC1EPFvPvES1_S1_>
    RETURN_IF(thread == nullptr, -0x01);
    80001670:	04090463          	beqz	s2,800016b8 <_ZN6kernel11SystemCalls13thread_createEv+0xb4>
    *handle = (thread_t) thread;
    80001674:	0129b023          	sd	s2,0(s3)
    Scheduler::getInstance().put(thread);
    80001678:	00000097          	auipc	ra,0x0
    8000167c:	528080e7          	jalr	1320(ra) # 80001ba0 <_ZN6kernel9Scheduler11getInstanceEv>
    80001680:	00090593          	mv	a1,s2
    80001684:	00000097          	auipc	ra,0x0
    80001688:	53c080e7          	jalr	1340(ra) # 80001bc0 <_ZN6kernel9Scheduler3putEPNS_3TCBE>
    registers.a0 = 0x00;
    8000168c:	0604b023          	sd	zero,96(s1)
}
    80001690:	03813083          	ld	ra,56(sp)
    80001694:	03013403          	ld	s0,48(sp)
    80001698:	02813483          	ld	s1,40(sp)
    8000169c:	02013903          	ld	s2,32(sp)
    800016a0:	01813983          	ld	s3,24(sp)
    800016a4:	01013a03          	ld	s4,16(sp)
    800016a8:	00813a83          	ld	s5,8(sp)
    800016ac:	00013b03          	ld	s6,0(sp)
    800016b0:	04010113          	addi	sp,sp,64
    800016b4:	00008067          	ret
    RETURN_IF(thread == nullptr, -0x01);
    800016b8:	fff00793          	li	a5,-1
    800016bc:	06f4b023          	sd	a5,96(s1)
    800016c0:	fd1ff06f          	j	80001690 <_ZN6kernel11SystemCalls13thread_createEv+0x8c>
    800016c4:	00050493          	mv	s1,a0
    auto thread = new TCB(task, argument, stack);
    800016c8:	00090513          	mv	a0,s2
    800016cc:	00001097          	auipc	ra,0x1
    800016d0:	850080e7          	jalr	-1968(ra) # 80001f1c <_ZN6kernel3TCBdlEPv>
    800016d4:	00048513          	mv	a0,s1
    800016d8:	0000e097          	auipc	ra,0xe
    800016dc:	b70080e7          	jalr	-1168(ra) # 8000f248 <_Unwind_Resume>

00000000800016e0 <_ZN6kernel11SystemCalls11thread_exitEv>:

void kernel::SystemCalls::thread_exit() { // Handle if attempting to exit main
    800016e0:	fe010113          	addi	sp,sp,-32
    800016e4:	00113c23          	sd	ra,24(sp)
    800016e8:	00813823          	sd	s0,16(sp)
    800016ec:	00913423          	sd	s1,8(sp)
    800016f0:	02010413          	addi	s0,sp,32
    auto runningThread = TCB::getRunningThread();
    800016f4:	00001097          	auipc	ra,0x1
    800016f8:	900080e7          	jalr	-1792(ra) # 80001ff4 <_ZN6kernel3TCB16getRunningThreadEv>
    800016fc:	00050493          	mv	s1,a0
    auto &registers = runningThread->getRegisters();
    RETURN_IF(runningThread == TCB::getMainThread(), -0x01);
    80001700:	00001097          	auipc	ra,0x1
    80001704:	b30080e7          	jalr	-1232(ra) # 80002230 <_ZN6kernel3TCB13getMainThreadEv>
    80001708:	04a48063          	beq	s1,a0,80001748 <_ZN6kernel11SystemCalls11thread_exitEv+0x68>
    delete runningThread;
    8000170c:	00048e63          	beqz	s1,80001728 <_ZN6kernel11SystemCalls11thread_exitEv+0x48>
    80001710:	00048513          	mv	a0,s1
    80001714:	00001097          	auipc	ra,0x1
    80001718:	97c080e7          	jalr	-1668(ra) # 80002090 <_ZN6kernel3TCBD1Ev>
    8000171c:	00048513          	mv	a0,s1
    80001720:	00000097          	auipc	ra,0x0
    80001724:	7fc080e7          	jalr	2044(ra) # 80001f1c <_ZN6kernel3TCBdlEPv>
    TCB::dispatch();
    80001728:	00001097          	auipc	ra,0x1
    8000172c:	84c080e7          	jalr	-1972(ra) # 80001f74 <_ZN6kernel3TCB8dispatchEv>
    registers.a0 = 0x00;
    80001730:	0604b023          	sd	zero,96(s1)
}
    80001734:	01813083          	ld	ra,24(sp)
    80001738:	01013403          	ld	s0,16(sp)
    8000173c:	00813483          	ld	s1,8(sp)
    80001740:	02010113          	addi	sp,sp,32
    80001744:	00008067          	ret
    RETURN_IF(runningThread == TCB::getMainThread(), -0x01);
    80001748:	fff00793          	li	a5,-1
    8000174c:	06f4b023          	sd	a5,96(s1)
    80001750:	fe5ff06f          	j	80001734 <_ZN6kernel11SystemCalls11thread_exitEv+0x54>

0000000080001754 <_ZN6kernel11SystemCalls8sem_openEv>:

void kernel::SystemCalls::sem_open() {
    80001754:	fd010113          	addi	sp,sp,-48
    80001758:	02113423          	sd	ra,40(sp)
    8000175c:	02813023          	sd	s0,32(sp)
    80001760:	00913c23          	sd	s1,24(sp)
    80001764:	01213823          	sd	s2,16(sp)
    80001768:	01313423          	sd	s3,8(sp)
    8000176c:	01413023          	sd	s4,0(sp)
    80001770:	03010413          	addi	s0,sp,48
    auto &registers = TCB::getRunningThread()->getRegisters();
    80001774:	00001097          	auipc	ra,0x1
    80001778:	880080e7          	jalr	-1920(ra) # 80001ff4 <_ZN6kernel3TCB16getRunningThreadEv>
    8000177c:	00050913          	mv	s2,a0
    auto init = (unsigned ) registers.a2;
    80001780:	07053a03          	ld	s4,112(a0)
    auto handle = (sem_t *) registers.a1;
    80001784:	06853983          	ld	s3,104(a0)
    auto semaphore = new Semaphore((int) init);
    80001788:	01800513          	li	a0,24
    8000178c:	00000097          	auipc	ra,0x0
    80001790:	174080e7          	jalr	372(ra) # 80001900 <_ZN6kernel9SemaphorenwEm>
    80001794:	00050493          	mv	s1,a0
    80001798:	00050863          	beqz	a0,800017a8 <_ZN6kernel11SystemCalls8sem_openEv+0x54>
    8000179c:	000a059b          	sext.w	a1,s4
    800017a0:	00000097          	auipc	ra,0x0
    800017a4:	1e8080e7          	jalr	488(ra) # 80001988 <_ZN6kernel9SemaphoreC1Ei>
    RETURN_IF(semaphore == nullptr, -0x01);
    800017a8:	02048663          	beqz	s1,800017d4 <_ZN6kernel11SystemCalls8sem_openEv+0x80>
    *handle = (sem_t) semaphore;
    800017ac:	0099b023          	sd	s1,0(s3)
    registers.a0 = 0x00;
    800017b0:	06093023          	sd	zero,96(s2)
}
    800017b4:	02813083          	ld	ra,40(sp)
    800017b8:	02013403          	ld	s0,32(sp)
    800017bc:	01813483          	ld	s1,24(sp)
    800017c0:	01013903          	ld	s2,16(sp)
    800017c4:	00813983          	ld	s3,8(sp)
    800017c8:	00013a03          	ld	s4,0(sp)
    800017cc:	03010113          	addi	sp,sp,48
    800017d0:	00008067          	ret
    RETURN_IF(semaphore == nullptr, -0x01);
    800017d4:	fff00793          	li	a5,-1
    800017d8:	06f93023          	sd	a5,96(s2)
    800017dc:	fd9ff06f          	j	800017b4 <_ZN6kernel11SystemCalls8sem_openEv+0x60>
    800017e0:	00050913          	mv	s2,a0
    auto semaphore = new Semaphore((int) init);
    800017e4:	00048513          	mv	a0,s1
    800017e8:	00000097          	auipc	ra,0x0
    800017ec:	160080e7          	jalr	352(ra) # 80001948 <_ZN6kernel9SemaphoredlEPv>
    800017f0:	00090513          	mv	a0,s2
    800017f4:	0000e097          	auipc	ra,0xe
    800017f8:	a54080e7          	jalr	-1452(ra) # 8000f248 <_Unwind_Resume>

00000000800017fc <_ZN6kernel11SystemCalls9sem_closeEv>:

void kernel::SystemCalls::sem_close() {
    800017fc:	fe010113          	addi	sp,sp,-32
    80001800:	00113c23          	sd	ra,24(sp)
    80001804:	00813823          	sd	s0,16(sp)
    80001808:	00913423          	sd	s1,8(sp)
    8000180c:	01213023          	sd	s2,0(sp)
    80001810:	02010413          	addi	s0,sp,32
    auto &registers = TCB::getRunningThread()->getRegisters();
    80001814:	00000097          	auipc	ra,0x0
    80001818:	7e0080e7          	jalr	2016(ra) # 80001ff4 <_ZN6kernel3TCB16getRunningThreadEv>
    8000181c:	00050493          	mv	s1,a0
    auto handle = (Semaphore*) registers.a1;
    80001820:	06853903          	ld	s2,104(a0)
    RETURN_IF(handle == nullptr, -0x01);
    80001824:	02091263          	bnez	s2,80001848 <_ZN6kernel11SystemCalls9sem_closeEv+0x4c>
    80001828:	fff00793          	li	a5,-1
    8000182c:	06f53023          	sd	a5,96(a0)
    delete handle;
    registers.a0 = 0x00;
}
    80001830:	01813083          	ld	ra,24(sp)
    80001834:	01013403          	ld	s0,16(sp)
    80001838:	00813483          	ld	s1,8(sp)
    8000183c:	00013903          	ld	s2,0(sp)
    80001840:	02010113          	addi	sp,sp,32
    80001844:	00008067          	ret
    delete handle;
    80001848:	00090513          	mv	a0,s2
    8000184c:	00000097          	auipc	ra,0x0
    80001850:	160080e7          	jalr	352(ra) # 800019ac <_ZN6kernel9SemaphoreD1Ev>
    80001854:	00090513          	mv	a0,s2
    80001858:	00000097          	auipc	ra,0x0
    8000185c:	0f0080e7          	jalr	240(ra) # 80001948 <_ZN6kernel9SemaphoredlEPv>
    registers.a0 = 0x00;
    80001860:	0604b023          	sd	zero,96(s1)
    80001864:	fcdff06f          	j	80001830 <_ZN6kernel11SystemCalls9sem_closeEv+0x34>

0000000080001868 <_ZN6kernel11SystemCalls8sem_waitEv>:

void kernel::SystemCalls::sem_wait() {
    80001868:	ff010113          	addi	sp,sp,-16
    8000186c:	00113423          	sd	ra,8(sp)
    80001870:	00813023          	sd	s0,0(sp)
    80001874:	01010413          	addi	s0,sp,16
    auto &registers = TCB::getRunningThread()->getRegisters();
    80001878:	00000097          	auipc	ra,0x0
    8000187c:	77c080e7          	jalr	1916(ra) # 80001ff4 <_ZN6kernel3TCB16getRunningThreadEv>
    80001880:	00050793          	mv	a5,a0
    auto id = (Semaphore*) registers.a1;
    80001884:	06853503          	ld	a0,104(a0)
    RETURN_IF(id == nullptr, -0x01);
    80001888:	00051e63          	bnez	a0,800018a4 <_ZN6kernel11SystemCalls8sem_waitEv+0x3c>
    8000188c:	fff00713          	li	a4,-1
    80001890:	06e7b023          	sd	a4,96(a5)
    registers.a0 = 0x00;
    id->wait();
}
    80001894:	00813083          	ld	ra,8(sp)
    80001898:	00013403          	ld	s0,0(sp)
    8000189c:	01010113          	addi	sp,sp,16
    800018a0:	00008067          	ret
    registers.a0 = 0x00;
    800018a4:	0607b023          	sd	zero,96(a5)
    id->wait();
    800018a8:	00000097          	auipc	ra,0x0
    800018ac:	16c080e7          	jalr	364(ra) # 80001a14 <_ZN6kernel9Semaphore4waitEv>
    800018b0:	fe5ff06f          	j	80001894 <_ZN6kernel11SystemCalls8sem_waitEv+0x2c>

00000000800018b4 <_ZN6kernel11SystemCalls10sem_signalEv>:

void kernel::SystemCalls::sem_signal() {
    800018b4:	ff010113          	addi	sp,sp,-16
    800018b8:	00113423          	sd	ra,8(sp)
    800018bc:	00813023          	sd	s0,0(sp)
    800018c0:	01010413          	addi	s0,sp,16
    auto &registers = TCB::getRunningThread()->getRegisters();
    800018c4:	00000097          	auipc	ra,0x0
    800018c8:	730080e7          	jalr	1840(ra) # 80001ff4 <_ZN6kernel3TCB16getRunningThreadEv>
    800018cc:	00050793          	mv	a5,a0
    auto id = (Semaphore*) registers.a1;
    800018d0:	06853503          	ld	a0,104(a0)
    RETURN_IF(id == nullptr, -0x01);
    800018d4:	00051e63          	bnez	a0,800018f0 <_ZN6kernel11SystemCalls10sem_signalEv+0x3c>
    800018d8:	fff00713          	li	a4,-1
    800018dc:	06e7b023          	sd	a4,96(a5)
    registers.a0 = 0x00;
    id->signal();
    800018e0:	00813083          	ld	ra,8(sp)
    800018e4:	00013403          	ld	s0,0(sp)
    800018e8:	01010113          	addi	sp,sp,16
    800018ec:	00008067          	ret
    registers.a0 = 0x00;
    800018f0:	0607b023          	sd	zero,96(a5)
    id->signal();
    800018f4:	00000097          	auipc	ra,0x0
    800018f8:	260080e7          	jalr	608(ra) # 80001b54 <_ZN6kernel9Semaphore6signalEv>
    800018fc:	fe5ff06f          	j	800018e0 <_ZN6kernel11SystemCalls10sem_signalEv+0x2c>

0000000080001900 <_ZN6kernel9SemaphorenwEm>:
#include "../../h/kernel/Scheduler.h"
#include "../../h/kernel/MemoryAllocator.h"

namespace kernel {

    void* Semaphore::operator new(size_t size) noexcept {
    80001900:	fe010113          	addi	sp,sp,-32
    80001904:	00113c23          	sd	ra,24(sp)
    80001908:	00813823          	sd	s0,16(sp)
    8000190c:	00913423          	sd	s1,8(sp)
    80001910:	02010413          	addi	s0,sp,32
        auto blocks = MemoryAllocator::byteSizeToBlockCount(size);
    80001914:	00001097          	auipc	ra,0x1
    80001918:	af0080e7          	jalr	-1296(ra) # 80002404 <_ZN6kernel15MemoryAllocator20byteSizeToBlockCountEm>
    8000191c:	00050493          	mv	s1,a0
        return MemoryAllocator::getInstance().allocateBlocks(blocks);
    80001920:	00001097          	auipc	ra,0x1
    80001924:	a88080e7          	jalr	-1400(ra) # 800023a8 <_ZN6kernel15MemoryAllocator11getInstanceEv>
    80001928:	00048593          	mv	a1,s1
    8000192c:	00001097          	auipc	ra,0x1
    80001930:	af8080e7          	jalr	-1288(ra) # 80002424 <_ZN6kernel15MemoryAllocator14allocateBlocksEm>
    }
    80001934:	01813083          	ld	ra,24(sp)
    80001938:	01013403          	ld	s0,16(sp)
    8000193c:	00813483          	ld	s1,8(sp)
    80001940:	02010113          	addi	sp,sp,32
    80001944:	00008067          	ret

0000000080001948 <_ZN6kernel9SemaphoredlEPv>:

    void Semaphore::operator delete(void *ptr) noexcept {
    80001948:	fe010113          	addi	sp,sp,-32
    8000194c:	00113c23          	sd	ra,24(sp)
    80001950:	00813823          	sd	s0,16(sp)
    80001954:	00913423          	sd	s1,8(sp)
    80001958:	02010413          	addi	s0,sp,32
    8000195c:	00050493          	mv	s1,a0
        MemoryAllocator::getInstance().deallocateBlocks(ptr);
    80001960:	00001097          	auipc	ra,0x1
    80001964:	a48080e7          	jalr	-1464(ra) # 800023a8 <_ZN6kernel15MemoryAllocator11getInstanceEv>
    80001968:	00048593          	mv	a1,s1
    8000196c:	00001097          	auipc	ra,0x1
    80001970:	b98080e7          	jalr	-1128(ra) # 80002504 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv>
    }
    80001974:	01813083          	ld	ra,24(sp)
    80001978:	01013403          	ld	s0,16(sp)
    8000197c:	00813483          	ld	s1,8(sp)
    80001980:	02010113          	addi	sp,sp,32
    80001984:	00008067          	ret

0000000080001988 <_ZN6kernel9SemaphoreC1Ei>:

    Semaphore::Semaphore(int value) :
    80001988:	ff010113          	addi	sp,sp,-16
    8000198c:	00813423          	sd	s0,8(sp)
    80001990:	01010413          	addi	s0,sp,16
        value(value) { }
    80001994:	00b53023          	sd	a1,0(a0)
    80001998:	00053423          	sd	zero,8(a0)
    8000199c:	00053823          	sd	zero,16(a0)
    800019a0:	00813403          	ld	s0,8(sp)
    800019a4:	01010113          	addi	sp,sp,16
    800019a8:	00008067          	ret

00000000800019ac <_ZN6kernel9SemaphoreD1Ev>:

    Semaphore::~Semaphore() {
    800019ac:	fe010113          	addi	sp,sp,-32
    800019b0:	00113c23          	sd	ra,24(sp)
    800019b4:	00813823          	sd	s0,16(sp)
    800019b8:	00913423          	sd	s1,8(sp)
    800019bc:	01213023          	sd	s2,0(sp)
    800019c0:	02010413          	addi	s0,sp,32
    800019c4:	00050493          	mv	s1,a0
        auto& scheduler = Scheduler::getInstance();
    800019c8:	00000097          	auipc	ra,0x0
    800019cc:	1d8080e7          	jalr	472(ra) # 80001ba0 <_ZN6kernel9Scheduler11getInstanceEv>
    800019d0:	00050913          	mv	s2,a0
        auto curr = head;
    800019d4:	0084b483          	ld	s1,8(s1)
        while(curr != nullptr) {
    800019d8:	02048263          	beqz	s1,800019fc <_ZN6kernel9SemaphoreD1Ev+0x50>
            curr->getRegisters().a0 = -0x02;
    800019dc:	ffe00793          	li	a5,-2
    800019e0:	06f4b023          	sd	a5,96(s1)
            scheduler.put(curr);
    800019e4:	00048593          	mv	a1,s1
    800019e8:	00090513          	mv	a0,s2
    800019ec:	00000097          	auipc	ra,0x0
    800019f0:	1d4080e7          	jalr	468(ra) # 80001bc0 <_ZN6kernel9Scheduler3putEPNS_3TCBE>
            curr=curr->next;
    800019f4:	1304b483          	ld	s1,304(s1)
        while(curr != nullptr) {
    800019f8:	fe1ff06f          	j	800019d8 <_ZN6kernel9SemaphoreD1Ev+0x2c>
        }
    }
    800019fc:	01813083          	ld	ra,24(sp)
    80001a00:	01013403          	ld	s0,16(sp)
    80001a04:	00813483          	ld	s1,8(sp)
    80001a08:	00013903          	ld	s2,0(sp)
    80001a0c:	02010113          	addi	sp,sp,32
    80001a10:	00008067          	ret

0000000080001a14 <_ZN6kernel9Semaphore4waitEv>:

    void Semaphore::wait() {
    80001a14:	ff010113          	addi	sp,sp,-16
    80001a18:	00813423          	sd	s0,8(sp)
    80001a1c:	01010413          	addi	s0,sp,16
        if(--value < 0) block();
    80001a20:	00053783          	ld	a5,0(a0)
    80001a24:	fff78793          	addi	a5,a5,-1
    80001a28:	00f53023          	sd	a5,0(a0)
    }
    80001a2c:	00813403          	ld	s0,8(sp)
    80001a30:	01010113          	addi	sp,sp,16
    80001a34:	00008067          	ret

0000000080001a38 <_ZN6kernel9Semaphore7enqueueEPNS_3TCBE>:
        auto thread = dequeue();
        if(thread == nullptr) return;
        Scheduler::getInstance().put(thread);
    }

    void Semaphore::enqueue(TCB *tcb) {
    80001a38:	ff010113          	addi	sp,sp,-16
    80001a3c:	00813423          	sd	s0,8(sp)
    80001a40:	01010413          	addi	s0,sp,16
        if(head == nullptr){
    80001a44:	00853783          	ld	a5,8(a0)
    80001a48:	00078e63          	beqz	a5,80001a64 <_ZN6kernel9Semaphore7enqueueEPNS_3TCBE+0x2c>
            head = tcb;
        }else{
            tail->next = tcb;
    80001a4c:	01053783          	ld	a5,16(a0)
    80001a50:	12b7b823          	sd	a1,304(a5)
        }
        tail = tcb;
    80001a54:	00b53823          	sd	a1,16(a0)
    }
    80001a58:	00813403          	ld	s0,8(sp)
    80001a5c:	01010113          	addi	sp,sp,16
    80001a60:	00008067          	ret
            head = tcb;
    80001a64:	00b53423          	sd	a1,8(a0)
    80001a68:	fedff06f          	j	80001a54 <_ZN6kernel9Semaphore7enqueueEPNS_3TCBE+0x1c>

0000000080001a6c <_ZN6kernel9Semaphore5blockEv>:
    void Semaphore::block() {
    80001a6c:	fe010113          	addi	sp,sp,-32
    80001a70:	00113c23          	sd	ra,24(sp)
    80001a74:	00813823          	sd	s0,16(sp)
    80001a78:	00913423          	sd	s1,8(sp)
    80001a7c:	02010413          	addi	s0,sp,32
    80001a80:	00050493          	mv	s1,a0
        auto thread = TCB::getRunningThread();
    80001a84:	00000097          	auipc	ra,0x0
    80001a88:	570080e7          	jalr	1392(ra) # 80001ff4 <_ZN6kernel3TCB16getRunningThreadEv>
    80001a8c:	00050593          	mv	a1,a0
        TCB::runningThread = nullptr;
    80001a90:	00004797          	auipc	a5,0x4
    80001a94:	6407b783          	ld	a5,1600(a5) # 800060d0 <_GLOBAL_OFFSET_TABLE_+0x18>
    80001a98:	0007b023          	sd	zero,0(a5)
        uint64 getThreadId() const { return id; }
        Registers& getRegisters() { return context.registers; }
        uint64* getPC() const { return context.pc; }
        void setPC(uint64* value) { context.pc = value; }
        uint64 getsstatus() const{ return context.sstatus; }
        void setStatus(ThreadStatus value) { status = value; };
    80001a9c:	00300793          	li	a5,3
    80001aa0:	14f52023          	sw	a5,320(a0)
        enqueue(thread);
    80001aa4:	00048513          	mv	a0,s1
    80001aa8:	00000097          	auipc	ra,0x0
    80001aac:	f90080e7          	jalr	-112(ra) # 80001a38 <_ZN6kernel9Semaphore7enqueueEPNS_3TCBE>
        TCB::dispatch();
    80001ab0:	00000097          	auipc	ra,0x0
    80001ab4:	4c4080e7          	jalr	1220(ra) # 80001f74 <_ZN6kernel3TCB8dispatchEv>
    }
    80001ab8:	01813083          	ld	ra,24(sp)
    80001abc:	01013403          	ld	s0,16(sp)
    80001ac0:	00813483          	ld	s1,8(sp)
    80001ac4:	02010113          	addi	sp,sp,32
    80001ac8:	00008067          	ret

0000000080001acc <_ZN6kernel9Semaphore7dequeueEv>:

    TCB *Semaphore::dequeue() {
    80001acc:	ff010113          	addi	sp,sp,-16
    80001ad0:	00813423          	sd	s0,8(sp)
    80001ad4:	01010413          	addi	s0,sp,16
    80001ad8:	00050793          	mv	a5,a0
        if(head == nullptr) return nullptr;
    80001adc:	00853503          	ld	a0,8(a0)
    80001ae0:	00050a63          	beqz	a0,80001af4 <_ZN6kernel9Semaphore7dequeueEv+0x28>
        auto tcb = head;
        head = head->next;
    80001ae4:	13053703          	ld	a4,304(a0)
    80001ae8:	00e7b423          	sd	a4,8(a5)
        if(head == nullptr){
    80001aec:	00070a63          	beqz	a4,80001b00 <_ZN6kernel9Semaphore7dequeueEv+0x34>
            tail = nullptr;
        }
        tcb->next= nullptr;
    80001af0:	12053823          	sd	zero,304(a0)
        return tcb;
    }
    80001af4:	00813403          	ld	s0,8(sp)
    80001af8:	01010113          	addi	sp,sp,16
    80001afc:	00008067          	ret
            tail = nullptr;
    80001b00:	0007b823          	sd	zero,16(a5)
    80001b04:	fedff06f          	j	80001af0 <_ZN6kernel9Semaphore7dequeueEv+0x24>

0000000080001b08 <_ZN6kernel9Semaphore7unblockEv>:
    void Semaphore::unblock() {
    80001b08:	fe010113          	addi	sp,sp,-32
    80001b0c:	00113c23          	sd	ra,24(sp)
    80001b10:	00813823          	sd	s0,16(sp)
    80001b14:	00913423          	sd	s1,8(sp)
    80001b18:	02010413          	addi	s0,sp,32
        auto thread = dequeue();
    80001b1c:	00000097          	auipc	ra,0x0
    80001b20:	fb0080e7          	jalr	-80(ra) # 80001acc <_ZN6kernel9Semaphore7dequeueEv>
        if(thread == nullptr) return;
    80001b24:	00050e63          	beqz	a0,80001b40 <_ZN6kernel9Semaphore7unblockEv+0x38>
    80001b28:	00050493          	mv	s1,a0
        Scheduler::getInstance().put(thread);
    80001b2c:	00000097          	auipc	ra,0x0
    80001b30:	074080e7          	jalr	116(ra) # 80001ba0 <_ZN6kernel9Scheduler11getInstanceEv>
    80001b34:	00048593          	mv	a1,s1
    80001b38:	00000097          	auipc	ra,0x0
    80001b3c:	088080e7          	jalr	136(ra) # 80001bc0 <_ZN6kernel9Scheduler3putEPNS_3TCBE>
    }
    80001b40:	01813083          	ld	ra,24(sp)
    80001b44:	01013403          	ld	s0,16(sp)
    80001b48:	00813483          	ld	s1,8(sp)
    80001b4c:	02010113          	addi	sp,sp,32
    80001b50:	00008067          	ret

0000000080001b54 <_ZN6kernel9Semaphore6signalEv>:
        if(++value<=0) unblock();
    80001b54:	00053783          	ld	a5,0(a0)
    80001b58:	00178793          	addi	a5,a5,1
    80001b5c:	00f53023          	sd	a5,0(a0)
    80001b60:	00078463          	beqz	a5,80001b68 <_ZN6kernel9Semaphore6signalEv+0x14>
    80001b64:	00008067          	ret
    void Semaphore::signal() {
    80001b68:	ff010113          	addi	sp,sp,-16
    80001b6c:	00113423          	sd	ra,8(sp)
    80001b70:	00813023          	sd	s0,0(sp)
    80001b74:	01010413          	addi	s0,sp,16
        if(++value<=0) unblock();
    80001b78:	00000097          	auipc	ra,0x0
    80001b7c:	f90080e7          	jalr	-112(ra) # 80001b08 <_ZN6kernel9Semaphore7unblockEv>
    }
    80001b80:	00813083          	ld	ra,8(sp)
    80001b84:	00013403          	ld	s0,0(sp)
    80001b88:	01010113          	addi	sp,sp,16
    80001b8c:	00008067          	ret

0000000080001b90 <_ZZN6kernel9Scheduler13getIdleThreadEvENUlPvE_4_FUNES1_>:
        }
    }

    TCB* Scheduler::getIdleThread() {
        auto& allocator = MemoryAllocator::getInstance();
        auto task = [](void*) {
    80001b90:	ff010113          	addi	sp,sp,-16
    80001b94:	00813423          	sd	s0,8(sp)
    80001b98:	01010413          	addi	s0,sp,16
            while(true);
    80001b9c:	0000006f          	j	80001b9c <_ZZN6kernel9Scheduler13getIdleThreadEvENUlPvE_4_FUNES1_+0xc>

0000000080001ba0 <_ZN6kernel9Scheduler11getInstanceEv>:
    Scheduler& kernel::Scheduler::getInstance() {
    80001ba0:	ff010113          	addi	sp,sp,-16
    80001ba4:	00813423          	sd	s0,8(sp)
    80001ba8:	01010413          	addi	s0,sp,16
    }
    80001bac:	00004517          	auipc	a0,0x4
    80001bb0:	58450513          	addi	a0,a0,1412 # 80006130 <_ZZN6kernel9Scheduler11getInstanceEvE8instance>
    80001bb4:	00813403          	ld	s0,8(sp)
    80001bb8:	01010113          	addi	sp,sp,16
    80001bbc:	00008067          	ret

0000000080001bc0 <_ZN6kernel9Scheduler3putEPNS_3TCBE>:
    void Scheduler::put(kernel::TCB *thread) {
    80001bc0:	ff010113          	addi	sp,sp,-16
    80001bc4:	00813423          	sd	s0,8(sp)
    80001bc8:	01010413          	addi	s0,sp,16
        if(thread == idleThread) return;
    80001bcc:	01053783          	ld	a5,16(a0)
    80001bd0:	02b78a63          	beq	a5,a1,80001c04 <_ZN6kernel9Scheduler3putEPNS_3TCBE+0x44>
        thread->status = TCB::ThreadStatus::READY;
    80001bd4:	00100793          	li	a5,1
    80001bd8:	14f5a023          	sw	a5,320(a1)
        if(readyHead == nullptr) {
    80001bdc:	00053783          	ld	a5,0(a0)
    80001be0:	02078863          	beqz	a5,80001c10 <_ZN6kernel9Scheduler3putEPNS_3TCBE+0x50>
            readyTail->next = thread;
    80001be4:	00853783          	ld	a5,8(a0)
    80001be8:	12b7b823          	sd	a1,304(a5)
        readyTail = thread;
    80001bec:	00b53423          	sd	a1,8(a0)
        uint64* getPC() const { return context.pc; }
        void setPC(uint64* value) { context.pc = value; }
        uint64 getsstatus() const{ return context.sstatus; }
        void setStatus(ThreadStatus value) { status = value; };
        ThreadStatus getStatus() { return status; };
        bool isUserThread() const { return type == ThreadType::USER; }
    80001bf0:	1285a783          	lw	a5,296(a1)
        if(thread->isUserThread()){
    80001bf4:	00079863          	bnez	a5,80001c04 <_ZN6kernel9Scheduler3putEPNS_3TCBE+0x44>
            userThreadCount++;
    80001bf8:	01853783          	ld	a5,24(a0)
    80001bfc:	00178793          	addi	a5,a5,1
    80001c00:	00f53c23          	sd	a5,24(a0)
    }
    80001c04:	00813403          	ld	s0,8(sp)
    80001c08:	01010113          	addi	sp,sp,16
    80001c0c:	00008067          	ret
            readyHead = thread;
    80001c10:	00b53023          	sd	a1,0(a0)
    80001c14:	fd9ff06f          	j	80001bec <_ZN6kernel9Scheduler3putEPNS_3TCBE+0x2c>

0000000080001c18 <_ZN6kernel9Scheduler13getIdleThreadEv>:
    TCB* Scheduler::getIdleThread() {
    80001c18:	fd010113          	addi	sp,sp,-48
    80001c1c:	02113423          	sd	ra,40(sp)
    80001c20:	02813023          	sd	s0,32(sp)
    80001c24:	00913c23          	sd	s1,24(sp)
    80001c28:	01213823          	sd	s2,16(sp)
    80001c2c:	01313423          	sd	s3,8(sp)
    80001c30:	03010413          	addi	s0,sp,48
    80001c34:	00050493          	mv	s1,a0
        auto& allocator = MemoryAllocator::getInstance();
    80001c38:	00000097          	auipc	ra,0x0
    80001c3c:	770080e7          	jalr	1904(ra) # 800023a8 <_ZN6kernel15MemoryAllocator11getInstanceEv>
        };
        if(!idleThread) {
    80001c40:	0104b783          	ld	a5,16(s1)
    80001c44:	02078263          	beqz	a5,80001c68 <_ZN6kernel9Scheduler13getIdleThreadEv+0x50>
            void* stack = allocator.allocateBytes(DEFAULT_STACK_SIZE);
            idleThread = new TCB(task, nullptr, stack, TCB::ThreadType::SYSTEM);
        }
        return idleThread;
    80001c48:	0104b503          	ld	a0,16(s1)
    }
    80001c4c:	02813083          	ld	ra,40(sp)
    80001c50:	02013403          	ld	s0,32(sp)
    80001c54:	01813483          	ld	s1,24(sp)
    80001c58:	01013903          	ld	s2,16(sp)
    80001c5c:	00813983          	ld	s3,8(sp)
    80001c60:	03010113          	addi	sp,sp,48
    80001c64:	00008067          	ret
            void* stack = allocator.allocateBytes(DEFAULT_STACK_SIZE);
    80001c68:	000015b7          	lui	a1,0x1
    80001c6c:	00001097          	auipc	ra,0x1
    80001c70:	a18080e7          	jalr	-1512(ra) # 80002684 <_ZN6kernel15MemoryAllocator13allocateBytesEm>
    80001c74:	00050993          	mv	s3,a0
            idleThread = new TCB(task, nullptr, stack, TCB::ThreadType::SYSTEM);
    80001c78:	14800513          	li	a0,328
    80001c7c:	00000097          	auipc	ra,0x0
    80001c80:	258080e7          	jalr	600(ra) # 80001ed4 <_ZN6kernel3TCBnwEm>
    80001c84:	00050913          	mv	s2,a0
    80001c88:	02050063          	beqz	a0,80001ca8 <_ZN6kernel9Scheduler13getIdleThreadEv+0x90>
    80001c8c:	00100713          	li	a4,1
    80001c90:	00098693          	mv	a3,s3
    80001c94:	00000613          	li	a2,0
    80001c98:	00000597          	auipc	a1,0x0
    80001c9c:	ef858593          	addi	a1,a1,-264 # 80001b90 <_ZZN6kernel9Scheduler13getIdleThreadEvENUlPvE_4_FUNES1_>
    80001ca0:	00000097          	auipc	ra,0x0
    80001ca4:	4bc080e7          	jalr	1212(ra) # 8000215c <_ZN6kernel3TCBC1EPFvPvES1_S1_NS0_10ThreadTypeE>
    80001ca8:	0124b823          	sd	s2,16(s1)
    80001cac:	f9dff06f          	j	80001c48 <_ZN6kernel9Scheduler13getIdleThreadEv+0x30>
    80001cb0:	00050493          	mv	s1,a0
    80001cb4:	00090513          	mv	a0,s2
    80001cb8:	00000097          	auipc	ra,0x0
    80001cbc:	264080e7          	jalr	612(ra) # 80001f1c <_ZN6kernel3TCBdlEPv>
    80001cc0:	00048513          	mv	a0,s1
    80001cc4:	0000d097          	auipc	ra,0xd
    80001cc8:	584080e7          	jalr	1412(ra) # 8000f248 <_Unwind_Resume>

0000000080001ccc <_ZN6kernel9Scheduler3getEv>:
    TCB *kernel::Scheduler::get() {
    80001ccc:	00050793          	mv	a5,a0
        if(readyHead == nullptr) return getIdleThread();
    80001cd0:	00053503          	ld	a0,0(a0)
    80001cd4:	02050663          	beqz	a0,80001d00 <_ZN6kernel9Scheduler3getEv+0x34>
        readyHead = readyHead->next;
    80001cd8:	13053703          	ld	a4,304(a0)
    80001cdc:	00e7b023          	sd	a4,0(a5)
        if(readyHead == nullptr) {
    80001ce0:	04070663          	beqz	a4,80001d2c <_ZN6kernel9Scheduler3getEv+0x60>
        thread->next= nullptr;
    80001ce4:	12053823          	sd	zero,304(a0)
    80001ce8:	12852703          	lw	a4,296(a0)
        if(thread->isUserThread()){
    80001cec:	04071463          	bnez	a4,80001d34 <_ZN6kernel9Scheduler3getEv+0x68>
            userThreadCount--;
    80001cf0:	0187b703          	ld	a4,24(a5)
    80001cf4:	fff70713          	addi	a4,a4,-1
    80001cf8:	00e7bc23          	sd	a4,24(a5)
    80001cfc:	00008067          	ret
    TCB *kernel::Scheduler::get() {
    80001d00:	ff010113          	addi	sp,sp,-16
    80001d04:	00113423          	sd	ra,8(sp)
    80001d08:	00813023          	sd	s0,0(sp)
    80001d0c:	01010413          	addi	s0,sp,16
        if(readyHead == nullptr) return getIdleThread();
    80001d10:	00078513          	mv	a0,a5
    80001d14:	00000097          	auipc	ra,0x0
    80001d18:	f04080e7          	jalr	-252(ra) # 80001c18 <_ZN6kernel9Scheduler13getIdleThreadEv>
    }
    80001d1c:	00813083          	ld	ra,8(sp)
    80001d20:	00013403          	ld	s0,0(sp)
    80001d24:	01010113          	addi	sp,sp,16
    80001d28:	00008067          	ret
            readyTail = nullptr;
    80001d2c:	0007b423          	sd	zero,8(a5)
    80001d30:	fb5ff06f          	j	80001ce4 <_ZN6kernel9Scheduler3getEv+0x18>
    }
    80001d34:	00008067          	ret

0000000080001d38 <_ZNK6kernel9Scheduler14hasUserThreadsEv>:

    bool Scheduler::hasUserThreads() const{
    80001d38:	ff010113          	addi	sp,sp,-16
    80001d3c:	00813423          	sd	s0,8(sp)
    80001d40:	01010413          	addi	s0,sp,16
        return userThreadCount != 0;
    80001d44:	01853503          	ld	a0,24(a0)
    }
    80001d48:	00a03533          	snez	a0,a0
    80001d4c:	00813403          	ld	s0,8(sp)
    80001d50:	01010113          	addi	sp,sp,16
    80001d54:	00008067          	ret

0000000080001d58 <_Z11printStringPKc>:
// Created by os on 7/18/22.
//
#include "../../h/kernel/ConsoleUtils.h"
#include "../../lib/console.h"

void printString(const char* text) {
    80001d58:	fe010113          	addi	sp,sp,-32
    80001d5c:	00113c23          	sd	ra,24(sp)
    80001d60:	00813823          	sd	s0,16(sp)
    80001d64:	00913423          	sd	s1,8(sp)
    80001d68:	02010413          	addi	s0,sp,32
    80001d6c:	00050493          	mv	s1,a0
    char* ptr = (char*)text;
    while(*ptr != '\0') __putc(*ptr++);
    80001d70:	0004c503          	lbu	a0,0(s1)
    80001d74:	00050a63          	beqz	a0,80001d88 <_Z11printStringPKc+0x30>
    80001d78:	00148493          	addi	s1,s1,1
    80001d7c:	00003097          	auipc	ra,0x3
    80001d80:	eb0080e7          	jalr	-336(ra) # 80004c2c <__putc>
    80001d84:	fedff06f          	j	80001d70 <_Z11printStringPKc+0x18>
}
    80001d88:	01813083          	ld	ra,24(sp)
    80001d8c:	01013403          	ld	s0,16(sp)
    80001d90:	00813483          	ld	s1,8(sp)
    80001d94:	02010113          	addi	sp,sp,32
    80001d98:	00008067          	ret

0000000080001d9c <_Z8printHexm>:

void printHex(uint64 num) {
    80001d9c:	fc010113          	addi	sp,sp,-64
    80001da0:	02113c23          	sd	ra,56(sp)
    80001da4:	02813823          	sd	s0,48(sp)
    80001da8:	02913423          	sd	s1,40(sp)
    80001dac:	04010413          	addi	s0,sp,64
    char str[20];
    str[0] = '0';
    80001db0:	03000793          	li	a5,48
    80001db4:	fcf40423          	sb	a5,-56(s0)
    str[1] = 'x';
    80001db8:	07800793          	li	a5,120
    80001dbc:	fcf404a3          	sb	a5,-55(s0)

    const char* digits = "0123456789ABCDEF";

    for(int i =0; i < 16; i++) {
    80001dc0:	00000713          	li	a4,0
    80001dc4:	00f00793          	li	a5,15
    80001dc8:	02e7cc63          	blt	a5,a4,80001e00 <_Z8printHexm+0x64>
        uint64 cdigit = num % 16;
    80001dcc:	00f57693          	andi	a3,a0,15
        str[17-i] = digits[cdigit];
    80001dd0:	01100793          	li	a5,17
    80001dd4:	40e787bb          	subw	a5,a5,a4
    80001dd8:	00003617          	auipc	a2,0x3
    80001ddc:	2f860613          	addi	a2,a2,760 # 800050d0 <CONSOLE_STATUS+0xc0>
    80001de0:	00c686b3          	add	a3,a3,a2
    80001de4:	0006c683          	lbu	a3,0(a3)
    80001de8:	fe040613          	addi	a2,s0,-32
    80001dec:	00f607b3          	add	a5,a2,a5
    80001df0:	fed78423          	sb	a3,-24(a5)
        num >>= 4;
    80001df4:	00455513          	srli	a0,a0,0x4
    for(int i =0; i < 16; i++) {
    80001df8:	0017071b          	addiw	a4,a4,1
    80001dfc:	fc9ff06f          	j	80001dc4 <_Z8printHexm+0x28>
    }

    str[18] = '\n';
    80001e00:	00a00793          	li	a5,10
    80001e04:	fcf40d23          	sb	a5,-38(s0)
    str[19] = '\0';
    80001e08:	fc040da3          	sb	zero,-37(s0)

    for(int i = 0 ; i < 19; i++){
    80001e0c:	00000493          	li	s1,0
    80001e10:	01200793          	li	a5,18
    80001e14:	0297c063          	blt	a5,s1,80001e34 <_Z8printHexm+0x98>
        __putc(str[i]);
    80001e18:	fe040793          	addi	a5,s0,-32
    80001e1c:	009787b3          	add	a5,a5,s1
    80001e20:	fe87c503          	lbu	a0,-24(a5)
    80001e24:	00003097          	auipc	ra,0x3
    80001e28:	e08080e7          	jalr	-504(ra) # 80004c2c <__putc>
    for(int i = 0 ; i < 19; i++){
    80001e2c:	0014849b          	addiw	s1,s1,1
    80001e30:	fe1ff06f          	j	80001e10 <_Z8printHexm+0x74>
    }
}
    80001e34:	03813083          	ld	ra,56(sp)
    80001e38:	03013403          	ld	s0,48(sp)
    80001e3c:	02813483          	ld	s1,40(sp)
    80001e40:	04010113          	addi	sp,sp,64
    80001e44:	00008067          	ret

0000000080001e48 <_Z8printRegPKcm>:

void printReg(const char* name, uint64 value) {
    80001e48:	fe010113          	addi	sp,sp,-32
    80001e4c:	00113c23          	sd	ra,24(sp)
    80001e50:	00813823          	sd	s0,16(sp)
    80001e54:	00913423          	sd	s1,8(sp)
    80001e58:	02010413          	addi	s0,sp,32
    80001e5c:	00058493          	mv	s1,a1
    printString(name);
    80001e60:	00000097          	auipc	ra,0x0
    80001e64:	ef8080e7          	jalr	-264(ra) # 80001d58 <_Z11printStringPKc>
    printString(": ");
    80001e68:	00003517          	auipc	a0,0x3
    80001e6c:	28050513          	addi	a0,a0,640 # 800050e8 <CONSOLE_STATUS+0xd8>
    80001e70:	00000097          	auipc	ra,0x0
    80001e74:	ee8080e7          	jalr	-280(ra) # 80001d58 <_Z11printStringPKc>
    printHex(value);
    80001e78:	00048513          	mv	a0,s1
    80001e7c:	00000097          	auipc	ra,0x0
    80001e80:	f20080e7          	jalr	-224(ra) # 80001d9c <_Z8printHexm>
}
    80001e84:	01813083          	ld	ra,24(sp)
    80001e88:	01013403          	ld	s0,16(sp)
    80001e8c:	00813483          	ld	s1,8(sp)
    80001e90:	02010113          	addi	sp,sp,32
    80001e94:	00008067          	ret

0000000080001e98 <_ZN6kernel3TCB11taskWrapperEv>:

    TCB* TCB::getRunningThread() {
        return runningThread;
    }

    void TCB::taskWrapper() {
    80001e98:	ff010113          	addi	sp,sp,-16
    80001e9c:	00113423          	sd	ra,8(sp)
    80001ea0:	00813023          	sd	s0,0(sp)
    80001ea4:	01010413          	addi	s0,sp,16
        runningThread->task(runningThread->arg);
    80001ea8:	00004797          	auipc	a5,0x4
    80001eac:	2a87b783          	ld	a5,680(a5) # 80006150 <__runningThread>
    80001eb0:	1107b703          	ld	a4,272(a5)
    80001eb4:	1187b503          	ld	a0,280(a5)
    80001eb8:	000700e7          	jalr	a4
        thread_exit();
    80001ebc:	fffff097          	auipc	ra,0xfffff
    80001ec0:	388080e7          	jalr	904(ra) # 80001244 <_Z11thread_exitv>
    }
    80001ec4:	00813083          	ld	ra,8(sp)
    80001ec8:	00013403          	ld	s0,0(sp)
    80001ecc:	01010113          	addi	sp,sp,16
    80001ed0:	00008067          	ret

0000000080001ed4 <_ZN6kernel3TCBnwEm>:
    void* TCB::operator new(size_t size) noexcept {
    80001ed4:	fe010113          	addi	sp,sp,-32
    80001ed8:	00113c23          	sd	ra,24(sp)
    80001edc:	00813823          	sd	s0,16(sp)
    80001ee0:	00913423          	sd	s1,8(sp)
    80001ee4:	02010413          	addi	s0,sp,32
        auto blocks = MemoryAllocator::byteSizeToBlockCount(size);
    80001ee8:	00000097          	auipc	ra,0x0
    80001eec:	51c080e7          	jalr	1308(ra) # 80002404 <_ZN6kernel15MemoryAllocator20byteSizeToBlockCountEm>
    80001ef0:	00050493          	mv	s1,a0
        return MemoryAllocator::getInstance().allocateBlocks(blocks);
    80001ef4:	00000097          	auipc	ra,0x0
    80001ef8:	4b4080e7          	jalr	1204(ra) # 800023a8 <_ZN6kernel15MemoryAllocator11getInstanceEv>
    80001efc:	00048593          	mv	a1,s1
    80001f00:	00000097          	auipc	ra,0x0
    80001f04:	524080e7          	jalr	1316(ra) # 80002424 <_ZN6kernel15MemoryAllocator14allocateBlocksEm>
    }
    80001f08:	01813083          	ld	ra,24(sp)
    80001f0c:	01013403          	ld	s0,16(sp)
    80001f10:	00813483          	ld	s1,8(sp)
    80001f14:	02010113          	addi	sp,sp,32
    80001f18:	00008067          	ret

0000000080001f1c <_ZN6kernel3TCBdlEPv>:
    void TCB::operator delete(void *ptr) noexcept {
    80001f1c:	fe010113          	addi	sp,sp,-32
    80001f20:	00113c23          	sd	ra,24(sp)
    80001f24:	00813823          	sd	s0,16(sp)
    80001f28:	00913423          	sd	s1,8(sp)
    80001f2c:	02010413          	addi	s0,sp,32
    80001f30:	00050493          	mv	s1,a0
        MemoryAllocator::getInstance().deallocateBlocks(ptr);
    80001f34:	00000097          	auipc	ra,0x0
    80001f38:	474080e7          	jalr	1140(ra) # 800023a8 <_ZN6kernel15MemoryAllocator11getInstanceEv>
    80001f3c:	00048593          	mv	a1,s1
    80001f40:	00000097          	auipc	ra,0x0
    80001f44:	5c4080e7          	jalr	1476(ra) # 80002504 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv>
    }
    80001f48:	01813083          	ld	ra,24(sp)
    80001f4c:	01013403          	ld	s0,16(sp)
    80001f50:	00813483          	ld	s1,8(sp)
    80001f54:	02010113          	addi	sp,sp,32
    80001f58:	00008067          	ret

0000000080001f5c <_ZN6kernel3TCB4tickEv>:
    void TCB::tick() {
    80001f5c:	ff010113          	addi	sp,sp,-16
    80001f60:	00813423          	sd	s0,8(sp)
    80001f64:	01010413          	addi	s0,sp,16
    }
    80001f68:	00813403          	ld	s0,8(sp)
    80001f6c:	01010113          	addi	sp,sp,16
    80001f70:	00008067          	ret

0000000080001f74 <_ZN6kernel3TCB8dispatchEv>:
    void TCB::dispatch() {
    80001f74:	fe010113          	addi	sp,sp,-32
    80001f78:	00113c23          	sd	ra,24(sp)
    80001f7c:	00813823          	sd	s0,16(sp)
    80001f80:	00913423          	sd	s1,8(sp)
    80001f84:	02010413          	addi	s0,sp,32
        auto& scheduler = Scheduler::getInstance();
    80001f88:	00000097          	auipc	ra,0x0
    80001f8c:	c18080e7          	jalr	-1000(ra) # 80001ba0 <_ZN6kernel9Scheduler11getInstanceEv>
    80001f90:	00050493          	mv	s1,a0
        auto oldThread = runningThread;
    80001f94:	00004597          	auipc	a1,0x4
    80001f98:	1bc5b583          	ld	a1,444(a1) # 80006150 <__runningThread>
        if (oldThread && oldThread->status != ThreadStatus::BLOCKED) {
    80001f9c:	00058863          	beqz	a1,80001fac <_ZN6kernel3TCB8dispatchEv+0x38>
    80001fa0:	1405a703          	lw	a4,320(a1)
    80001fa4:	00300793          	li	a5,3
    80001fa8:	04f71063          	bne	a4,a5,80001fe8 <_ZN6kernel3TCB8dispatchEv+0x74>
        auto newThread = scheduler.get();
    80001fac:	00048513          	mv	a0,s1
    80001fb0:	00000097          	auipc	ra,0x0
    80001fb4:	d1c080e7          	jalr	-740(ra) # 80001ccc <_ZN6kernel9Scheduler3getEv>
        runningThread = newThread;
    80001fb8:	00004797          	auipc	a5,0x4
    80001fbc:	18a7bc23          	sd	a0,408(a5) # 80006150 <__runningThread>
        runningThread->status = ThreadStatus::RUNNING;
    80001fc0:	00200793          	li	a5,2
    80001fc4:	14f52023          	sw	a5,320(a0)
        runningTimeLeft = DEFAULT_TIME_SLICE;
    80001fc8:	00200793          	li	a5,2
    80001fcc:	00004717          	auipc	a4,0x4
    80001fd0:	0cf73e23          	sd	a5,220(a4) # 800060a8 <_ZN6kernel3TCB15runningTimeLeftE>
    }
    80001fd4:	01813083          	ld	ra,24(sp)
    80001fd8:	01013403          	ld	s0,16(sp)
    80001fdc:	00813483          	ld	s1,8(sp)
    80001fe0:	02010113          	addi	sp,sp,32
    80001fe4:	00008067          	ret
            scheduler.put(oldThread);
    80001fe8:	00000097          	auipc	ra,0x0
    80001fec:	bd8080e7          	jalr	-1064(ra) # 80001bc0 <_ZN6kernel9Scheduler3putEPNS_3TCBE>
    80001ff0:	fbdff06f          	j	80001fac <_ZN6kernel3TCB8dispatchEv+0x38>

0000000080001ff4 <_ZN6kernel3TCB16getRunningThreadEv>:
    TCB* TCB::getRunningThread() {
    80001ff4:	ff010113          	addi	sp,sp,-16
    80001ff8:	00813423          	sd	s0,8(sp)
    80001ffc:	01010413          	addi	s0,sp,16
    }
    80002000:	00004517          	auipc	a0,0x4
    80002004:	15053503          	ld	a0,336(a0) # 80006150 <__runningThread>
    80002008:	00813403          	ld	s0,8(sp)
    8000200c:	01010113          	addi	sp,sp,16
    80002010:	00008067          	ret

0000000080002014 <_ZN6kernel3TCB17sstatusGetInitialEv>:

    uint64 TCB::sstatusGetInitial() {
    80002014:	ff010113          	addi	sp,sp,-16
    80002018:	00813423          	sd	s0,8(sp)
    8000201c:	01010413          	addi	s0,sp,16
        if(runningThread == nullptr) return 0;
    80002020:	00004797          	auipc	a5,0x4
    80002024:	1307b783          	ld	a5,304(a5) # 80006150 <__runningThread>
    80002028:	00078c63          	beqz	a5,80002040 <_ZN6kernel3TCB17sstatusGetInitialEv+0x2c>
        uint64 getsstatus() const{ return context.sstatus; }
    8000202c:	0087b503          	ld	a0,8(a5)
        using BitMasks::sstatus;
        auto mask = (uint64)sstatus::SPP | (uint64) sstatus::SPIE;
        return runningThread->getsstatus() & mask;
    80002030:	12057513          	andi	a0,a0,288
    }
    80002034:	00813403          	ld	s0,8(sp)
    80002038:	01010113          	addi	sp,sp,16
    8000203c:	00008067          	ret
        if(runningThread == nullptr) return 0;
    80002040:	00000513          	li	a0,0
    80002044:	ff1ff06f          	j	80002034 <_ZN6kernel3TCB17sstatusGetInitialEv+0x20>

0000000080002048 <_ZN6kernel3TCB12pcGetInitialEPFvPvE>:

    uint64* TCB::pcGetInitial(ThreadTask function) {
    80002048:	ff010113          	addi	sp,sp,-16
    8000204c:	00813423          	sd	s0,8(sp)
    80002050:	01010413          	addi	s0,sp,16
        if(function == nullptr) return nullptr;
    80002054:	00050c63          	beqz	a0,8000206c <_ZN6kernel3TCB12pcGetInitialEPFvPvE+0x24>
        return (uint64*) taskWrapper;
    80002058:	00000517          	auipc	a0,0x0
    8000205c:	e4050513          	addi	a0,a0,-448 # 80001e98 <_ZN6kernel3TCB11taskWrapperEv>
    }
    80002060:	00813403          	ld	s0,8(sp)
    80002064:	01010113          	addi	sp,sp,16
    80002068:	00008067          	ret
        if(function == nullptr) return nullptr;
    8000206c:	00000513          	li	a0,0
    80002070:	ff1ff06f          	j	80002060 <_ZN6kernel3TCB12pcGetInitialEPFvPvE+0x18>

0000000080002074 <_ZN6kernel3TCB17runningThreadTypeEv>:

    /* Return to old version once user mode is the default mode */
    TCB::ThreadType TCB::runningThreadType() {
    80002074:	ff010113          	addi	sp,sp,-16
    80002078:	00813423          	sd	s0,8(sp)
    8000207c:	01010413          	addi	s0,sp,16
        //auto status = runningThread->getsstatus();
        //return status | (uint64) BitMasks::sstatus::SPP? ThreadType::SYSTEM:ThreadType::USER;
        return ThreadType::USER;
    }
    80002080:	00000513          	li	a0,0
    80002084:	00813403          	ld	s0,8(sp)
    80002088:	01010113          	addi	sp,sp,16
    8000208c:	00008067          	ret

0000000080002090 <_ZN6kernel3TCBD1Ev>:
            auto stackTop = (uint64) &this->stack[DEFAULT_STACK_SIZE];
            context.registers.sp = stackTop;
        }
    }

    TCB::~TCB() {
    80002090:	fe010113          	addi	sp,sp,-32
    80002094:	00113c23          	sd	ra,24(sp)
    80002098:	00813823          	sd	s0,16(sp)
    8000209c:	00913423          	sd	s1,8(sp)
    800020a0:	02010413          	addi	s0,sp,32
    800020a4:	00050493          	mv	s1,a0
        if(stack != nullptr) {
    800020a8:	12053783          	ld	a5,288(a0)
    800020ac:	00078c63          	beqz	a5,800020c4 <_ZN6kernel3TCBD1Ev+0x34>
            auto& allocator = MemoryAllocator::getInstance();
    800020b0:	00000097          	auipc	ra,0x0
    800020b4:	2f8080e7          	jalr	760(ra) # 800023a8 <_ZN6kernel15MemoryAllocator11getInstanceEv>
            allocator.deallocateBlocks(stack);
    800020b8:	1204b583          	ld	a1,288(s1)
    800020bc:	00000097          	auipc	ra,0x0
    800020c0:	448080e7          	jalr	1096(ra) # 80002504 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv>
        }
        if(this == runningThread) runningThread = nullptr;
    800020c4:	00004797          	auipc	a5,0x4
    800020c8:	08c7b783          	ld	a5,140(a5) # 80006150 <__runningThread>
    800020cc:	00978c63          	beq	a5,s1,800020e4 <_ZN6kernel3TCBD1Ev+0x54>
    }
    800020d0:	01813083          	ld	ra,24(sp)
    800020d4:	01013403          	ld	s0,16(sp)
    800020d8:	00813483          	ld	s1,8(sp)
    800020dc:	02010113          	addi	sp,sp,32
    800020e0:	00008067          	ret
        if(this == runningThread) runningThread = nullptr;
    800020e4:	00004797          	auipc	a5,0x4
    800020e8:	0607b623          	sd	zero,108(a5) # 80006150 <__runningThread>
    }
    800020ec:	fe5ff06f          	j	800020d0 <_ZN6kernel3TCBD1Ev+0x40>

00000000800020f0 <_ZN6kernel3TCB9RegistersC1Ev>:

    TCB::ThreadContext::ThreadContext(uint64 status, uint64* pc) :
            pc(pc), sstatus(status), registers(){ }

    TCB::Registers::Registers() {
    800020f0:	ff010113          	addi	sp,sp,-16
    800020f4:	00813423          	sd	s0,8(sp)
    800020f8:	01010413          	addi	s0,sp,16
        for(int i = 0; i < 32; i++){
    800020fc:	00000793          	li	a5,0
    80002100:	01f00713          	li	a4,31
    80002104:	00f74c63          	blt	a4,a5,8000211c <_ZN6kernel3TCB9RegistersC1Ev+0x2c>
            ((uint64*)this)[i] = 0;
    80002108:	00379713          	slli	a4,a5,0x3
    8000210c:	00e50733          	add	a4,a0,a4
    80002110:	00073023          	sd	zero,0(a4)
        for(int i = 0; i < 32; i++){
    80002114:	0017879b          	addiw	a5,a5,1
    80002118:	fe9ff06f          	j	80002100 <_ZN6kernel3TCB9RegistersC1Ev+0x10>
        }
    }
    8000211c:	00813403          	ld	s0,8(sp)
    80002120:	01010113          	addi	sp,sp,16
    80002124:	00008067          	ret

0000000080002128 <_ZN6kernel3TCB13ThreadContextC1EmPm>:
    TCB::ThreadContext::ThreadContext(uint64 status, uint64* pc) :
    80002128:	ff010113          	addi	sp,sp,-16
    8000212c:	00113423          	sd	ra,8(sp)
    80002130:	00813023          	sd	s0,0(sp)
    80002134:	01010413          	addi	s0,sp,16
            pc(pc), sstatus(status), registers(){ }
    80002138:	00c53023          	sd	a2,0(a0)
    8000213c:	00b53423          	sd	a1,8(a0)
    80002140:	01050513          	addi	a0,a0,16
    80002144:	00000097          	auipc	ra,0x0
    80002148:	fac080e7          	jalr	-84(ra) # 800020f0 <_ZN6kernel3TCB9RegistersC1Ev>
    8000214c:	00813083          	ld	ra,8(sp)
    80002150:	00013403          	ld	s0,0(sp)
    80002154:	01010113          	addi	sp,sp,16
    80002158:	00008067          	ret

000000008000215c <_ZN6kernel3TCBC1EPFvPvES1_S1_NS0_10ThreadTypeE>:
    TCB::TCB(ThreadTask function, void *argument, void *stack, ThreadType type) :
    8000215c:	fc010113          	addi	sp,sp,-64
    80002160:	02113c23          	sd	ra,56(sp)
    80002164:	02813823          	sd	s0,48(sp)
    80002168:	02913423          	sd	s1,40(sp)
    8000216c:	03213023          	sd	s2,32(sp)
    80002170:	01313c23          	sd	s3,24(sp)
    80002174:	01413823          	sd	s4,16(sp)
    80002178:	01513423          	sd	s5,8(sp)
    8000217c:	01613023          	sd	s6,0(sp)
    80002180:	04010413          	addi	s0,sp,64
    80002184:	00050493          	mv	s1,a0
    80002188:	00058993          	mv	s3,a1
    8000218c:	00060a93          	mv	s5,a2
    80002190:	00068913          	mv	s2,a3
    80002194:	00070a13          	mv	s4,a4
            type(type) {
    80002198:	00000097          	auipc	ra,0x0
    8000219c:	e7c080e7          	jalr	-388(ra) # 80002014 <_ZN6kernel3TCB17sstatusGetInitialEv>
    800021a0:	00050b13          	mv	s6,a0
    800021a4:	00098513          	mv	a0,s3
    800021a8:	00000097          	auipc	ra,0x0
    800021ac:	ea0080e7          	jalr	-352(ra) # 80002048 <_ZN6kernel3TCB12pcGetInitialEPFvPvE>
    800021b0:	00050613          	mv	a2,a0
    800021b4:	000b0593          	mv	a1,s6
    800021b8:	00048513          	mv	a0,s1
    800021bc:	00000097          	auipc	ra,0x0
    800021c0:	f6c080e7          	jalr	-148(ra) # 80002128 <_ZN6kernel3TCB13ThreadContextC1EmPm>
    800021c4:	1134b823          	sd	s3,272(s1)
    800021c8:	1154bc23          	sd	s5,280(s1)
    800021cc:	1324b023          	sd	s2,288(s1)
    800021d0:	1344a423          	sw	s4,296(s1)
    800021d4:	1204b823          	sd	zero,304(s1)
    800021d8:	00004717          	auipc	a4,0x4
    800021dc:	f7870713          	addi	a4,a4,-136 # 80006150 <__runningThread>
    800021e0:	00873783          	ld	a5,8(a4)
    800021e4:	00178693          	addi	a3,a5,1
    800021e8:	00d73423          	sd	a3,8(a4)
    800021ec:	12f4bc23          	sd	a5,312(s1)
    800021f0:	00100793          	li	a5,1
    800021f4:	14f4a023          	sw	a5,320(s1)
        if(stack != nullptr) {
    800021f8:	00090863          	beqz	s2,80002208 <_ZN6kernel3TCBC1EPFvPvES1_S1_NS0_10ThreadTypeE+0xac>
            auto stackTop = (uint64) &this->stack[DEFAULT_STACK_SIZE];
    800021fc:	000086b7          	lui	a3,0x8
    80002200:	00d90933          	add	s2,s2,a3
            context.registers.sp = stackTop;
    80002204:	0324b023          	sd	s2,32(s1)
    }
    80002208:	03813083          	ld	ra,56(sp)
    8000220c:	03013403          	ld	s0,48(sp)
    80002210:	02813483          	ld	s1,40(sp)
    80002214:	02013903          	ld	s2,32(sp)
    80002218:	01813983          	ld	s3,24(sp)
    8000221c:	01013a03          	ld	s4,16(sp)
    80002220:	00813a83          	ld	s5,8(sp)
    80002224:	00013b03          	ld	s6,0(sp)
    80002228:	04010113          	addi	sp,sp,64
    8000222c:	00008067          	ret

0000000080002230 <_ZN6kernel3TCB13getMainThreadEv>:
        if(mainThread == nullptr) {
    80002230:	00004797          	auipc	a5,0x4
    80002234:	f307b783          	ld	a5,-208(a5) # 80006160 <_ZN6kernel3TCB10mainThreadE>
    80002238:	00078863          	beqz	a5,80002248 <_ZN6kernel3TCB13getMainThreadEv+0x18>
    }
    8000223c:	00004517          	auipc	a0,0x4
    80002240:	f2453503          	ld	a0,-220(a0) # 80006160 <_ZN6kernel3TCB10mainThreadE>
    80002244:	00008067          	ret
    TCB* TCB::getMainThread() {
    80002248:	fe010113          	addi	sp,sp,-32
    8000224c:	00113c23          	sd	ra,24(sp)
    80002250:	00813823          	sd	s0,16(sp)
    80002254:	00913423          	sd	s1,8(sp)
    80002258:	02010413          	addi	s0,sp,32
            mainThread = new TCB(nullptr, nullptr, nullptr, ThreadType::USER);
    8000225c:	14800513          	li	a0,328
    80002260:	00000097          	auipc	ra,0x0
    80002264:	c74080e7          	jalr	-908(ra) # 80001ed4 <_ZN6kernel3TCBnwEm>
    80002268:	00050493          	mv	s1,a0
    8000226c:	00050e63          	beqz	a0,80002288 <_ZN6kernel3TCB13getMainThreadEv+0x58>
    80002270:	00000713          	li	a4,0
    80002274:	00000693          	li	a3,0
    80002278:	00000613          	li	a2,0
    8000227c:	00000593          	li	a1,0
    80002280:	00000097          	auipc	ra,0x0
    80002284:	edc080e7          	jalr	-292(ra) # 8000215c <_ZN6kernel3TCBC1EPFvPvES1_S1_NS0_10ThreadTypeE>
    80002288:	00004797          	auipc	a5,0x4
    8000228c:	ec97bc23          	sd	s1,-296(a5) # 80006160 <_ZN6kernel3TCB10mainThreadE>
    }
    80002290:	00004517          	auipc	a0,0x4
    80002294:	ed053503          	ld	a0,-304(a0) # 80006160 <_ZN6kernel3TCB10mainThreadE>
    80002298:	01813083          	ld	ra,24(sp)
    8000229c:	01013403          	ld	s0,16(sp)
    800022a0:	00813483          	ld	s1,8(sp)
    800022a4:	02010113          	addi	sp,sp,32
    800022a8:	00008067          	ret

00000000800022ac <_ZN6kernel3TCB10initializeEv>:
    void TCB::initialize() {
    800022ac:	ff010113          	addi	sp,sp,-16
    800022b0:	00113423          	sd	ra,8(sp)
    800022b4:	00813023          	sd	s0,0(sp)
    800022b8:	01010413          	addi	s0,sp,16
        runningThread = getMainThread();
    800022bc:	00000097          	auipc	ra,0x0
    800022c0:	f74080e7          	jalr	-140(ra) # 80002230 <_ZN6kernel3TCB13getMainThreadEv>
    800022c4:	00004797          	auipc	a5,0x4
    800022c8:	e8a7b623          	sd	a0,-372(a5) # 80006150 <__runningThread>
        runningTimeLeft = DEFAULT_TIME_SLICE;
    800022cc:	00200793          	li	a5,2
    800022d0:	00004717          	auipc	a4,0x4
    800022d4:	dcf73c23          	sd	a5,-552(a4) # 800060a8 <_ZN6kernel3TCB15runningTimeLeftE>
    }
    800022d8:	00813083          	ld	ra,8(sp)
    800022dc:	00013403          	ld	s0,0(sp)
    800022e0:	01010113          	addi	sp,sp,16
    800022e4:	00008067          	ret

00000000800022e8 <_ZN6kernel3TCBC1EPFvPvES1_S1_>:
    TCB::TCB(ThreadTask function, void *argument, void *stack) :
    800022e8:	fd010113          	addi	sp,sp,-48
    800022ec:	02113423          	sd	ra,40(sp)
    800022f0:	02813023          	sd	s0,32(sp)
    800022f4:	00913c23          	sd	s1,24(sp)
    800022f8:	01213823          	sd	s2,16(sp)
    800022fc:	01313423          	sd	s3,8(sp)
    80002300:	01413023          	sd	s4,0(sp)
    80002304:	03010413          	addi	s0,sp,48
    80002308:	00050493          	mv	s1,a0
    8000230c:	00058913          	mv	s2,a1
    80002310:	00060993          	mv	s3,a2
    80002314:	00068a13          	mv	s4,a3
            TCB::TCB(function,argument,stack, runningThreadType()){ }
    80002318:	00000097          	auipc	ra,0x0
    8000231c:	d5c080e7          	jalr	-676(ra) # 80002074 <_ZN6kernel3TCB17runningThreadTypeEv>
    80002320:	00050713          	mv	a4,a0
    80002324:	000a0693          	mv	a3,s4
    80002328:	00098613          	mv	a2,s3
    8000232c:	00090593          	mv	a1,s2
    80002330:	00048513          	mv	a0,s1
    80002334:	00000097          	auipc	ra,0x0
    80002338:	e28080e7          	jalr	-472(ra) # 8000215c <_ZN6kernel3TCBC1EPFvPvES1_S1_NS0_10ThreadTypeE>
    8000233c:	02813083          	ld	ra,40(sp)
    80002340:	02013403          	ld	s0,32(sp)
    80002344:	01813483          	ld	s1,24(sp)
    80002348:	01013903          	ld	s2,16(sp)
    8000234c:	00813983          	ld	s3,8(sp)
    80002350:	00013a03          	ld	s4,0(sp)
    80002354:	03010113          	addi	sp,sp,48
    80002358:	00008067          	ret

000000008000235c <_ZN6kernel15MemoryAllocatorC1Ev>:
//

#include "../../h/kernel/MemoryAllocator.h"

namespace kernel {
    MemoryAllocator::MemoryAllocator() {
    8000235c:	ff010113          	addi	sp,sp,-16
    80002360:	00813423          	sd	s0,8(sp)
    80002364:	01010413          	addi	s0,sp,16
        size_t heapByteSize = (uint64) HEAP_END_ADDR - (uint64) HEAP_START_ADDR;
    80002368:	00004797          	auipc	a5,0x4
    8000236c:	d707b783          	ld	a5,-656(a5) # 800060d8 <_GLOBAL_OFFSET_TABLE_+0x20>
    80002370:	0007b783          	ld	a5,0(a5)
    80002374:	00004717          	auipc	a4,0x4
    80002378:	d4c73703          	ld	a4,-692(a4) # 800060c0 <_GLOBAL_OFFSET_TABLE_+0x8>
    8000237c:	00073703          	ld	a4,0(a4)
    80002380:	40e787b3          	sub	a5,a5,a4
        size_t heapBlockCount = heapByteSize / MEM_BLOCK_SIZE;
    80002384:	0067d793          	srli	a5,a5,0x6
        auto originBlock = (MemoryAllocator::FreeMemoryBlock *) HEAP_START_ADDR;
        originBlock->size = heapBlockCount - sizeof(FreeMemoryBlock);
    80002388:	fe878793          	addi	a5,a5,-24
    8000238c:	00f73823          	sd	a5,16(a4)
        originBlock->prev = nullptr;
    80002390:	00073023          	sd	zero,0(a4)
        originBlock->next = nullptr;
    80002394:	00073423          	sd	zero,8(a4)
        this->freeMemoryHead = originBlock;
    80002398:	00e53023          	sd	a4,0(a0)
    }
    8000239c:	00813403          	ld	s0,8(sp)
    800023a0:	01010113          	addi	sp,sp,16
    800023a4:	00008067          	ret

00000000800023a8 <_ZN6kernel15MemoryAllocator11getInstanceEv>:

    MemoryAllocator &MemoryAllocator::getInstance() {
        static MemoryAllocator allocatorInstance{};
    800023a8:	0000c797          	auipc	a5,0xc
    800023ac:	dc07c783          	lbu	a5,-576(a5) # 8000e168 <_ZGVZN6kernel15MemoryAllocator11getInstanceEvE17allocatorInstance>
    800023b0:	00078863          	beqz	a5,800023c0 <_ZN6kernel15MemoryAllocator11getInstanceEv+0x18>
        return allocatorInstance;
    }
    800023b4:	0000c517          	auipc	a0,0xc
    800023b8:	dbc50513          	addi	a0,a0,-580 # 8000e170 <_ZZN6kernel15MemoryAllocator11getInstanceEvE17allocatorInstance>
    800023bc:	00008067          	ret
    MemoryAllocator &MemoryAllocator::getInstance() {
    800023c0:	ff010113          	addi	sp,sp,-16
    800023c4:	00113423          	sd	ra,8(sp)
    800023c8:	00813023          	sd	s0,0(sp)
    800023cc:	01010413          	addi	s0,sp,16
        static MemoryAllocator allocatorInstance{};
    800023d0:	0000c517          	auipc	a0,0xc
    800023d4:	da050513          	addi	a0,a0,-608 # 8000e170 <_ZZN6kernel15MemoryAllocator11getInstanceEvE17allocatorInstance>
    800023d8:	00000097          	auipc	ra,0x0
    800023dc:	f84080e7          	jalr	-124(ra) # 8000235c <_ZN6kernel15MemoryAllocatorC1Ev>
    800023e0:	00100793          	li	a5,1
    800023e4:	0000c717          	auipc	a4,0xc
    800023e8:	d8f70223          	sb	a5,-636(a4) # 8000e168 <_ZGVZN6kernel15MemoryAllocator11getInstanceEvE17allocatorInstance>
    }
    800023ec:	0000c517          	auipc	a0,0xc
    800023f0:	d8450513          	addi	a0,a0,-636 # 8000e170 <_ZZN6kernel15MemoryAllocator11getInstanceEvE17allocatorInstance>
    800023f4:	00813083          	ld	ra,8(sp)
    800023f8:	00013403          	ld	s0,0(sp)
    800023fc:	01010113          	addi	sp,sp,16
    80002400:	00008067          	ret

0000000080002404 <_ZN6kernel15MemoryAllocator20byteSizeToBlockCountEm>:

    size_t MemoryAllocator::byteSizeToBlockCount(size_t blocks) {
    80002404:	ff010113          	addi	sp,sp,-16
    80002408:	00813423          	sd	s0,8(sp)
    8000240c:	01010413          	addi	s0,sp,16
        return (blocks + sizeof(size_t) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    80002410:	04750513          	addi	a0,a0,71
    }
    80002414:	00655513          	srli	a0,a0,0x6
    80002418:	00813403          	ld	s0,8(sp)
    8000241c:	01010113          	addi	sp,sp,16
    80002420:	00008067          	ret

0000000080002424 <_ZN6kernel15MemoryAllocator14allocateBlocksEm>:

/* Algorithm: Best Fit*/
    void *MemoryAllocator::allocateBlocks(size_t blockCount) {
    80002424:	ff010113          	addi	sp,sp,-16
    80002428:	00813423          	sd	s0,8(sp)
    8000242c:	01010413          	addi	s0,sp,16
    80002430:	00050613          	mv	a2,a0
        FreeMemoryBlock *block = nullptr;

        for (auto curr = this->freeMemoryHead; curr != nullptr; curr = curr->next) {
    80002434:	00053783          	ld	a5,0(a0)
        FreeMemoryBlock *block = nullptr;
    80002438:	00000513          	li	a0,0
    8000243c:	0140006f          	j	80002450 <_ZN6kernel15MemoryAllocator14allocateBlocksEm+0x2c>
            if (block == nullptr && curr->size >= blockCount) {
    80002440:	0107b703          	ld	a4,16(a5)
    80002444:	00b76a63          	bltu	a4,a1,80002458 <_ZN6kernel15MemoryAllocator14allocateBlocksEm+0x34>
                block = curr;
    80002448:	00078513          	mv	a0,a5
        for (auto curr = this->freeMemoryHead; curr != nullptr; curr = curr->next) {
    8000244c:	0087b783          	ld	a5,8(a5)
    80002450:	02078063          	beqz	a5,80002470 <_ZN6kernel15MemoryAllocator14allocateBlocksEm+0x4c>
            if (block == nullptr && curr->size >= blockCount) {
    80002454:	fe0506e3          	beqz	a0,80002440 <_ZN6kernel15MemoryAllocator14allocateBlocksEm+0x1c>
            } else if (curr->size >= blockCount && curr->size < block->size) {
    80002458:	0107b703          	ld	a4,16(a5)
    8000245c:	feb768e3          	bltu	a4,a1,8000244c <_ZN6kernel15MemoryAllocator14allocateBlocksEm+0x28>
    80002460:	01053683          	ld	a3,16(a0)
    80002464:	fed774e3          	bgeu	a4,a3,8000244c <_ZN6kernel15MemoryAllocator14allocateBlocksEm+0x28>
                block = curr;
    80002468:	00078513          	mv	a0,a5
    8000246c:	fe1ff06f          	j	8000244c <_ZN6kernel15MemoryAllocator14allocateBlocksEm+0x28>
            }
        }

        if (block == nullptr) {
    80002470:	04050863          	beqz	a0,800024c0 <_ZN6kernel15MemoryAllocator14allocateBlocksEm+0x9c>
            return nullptr;
        }

        size_t remainingBlockCount = block->size - blockCount;
    80002474:	01053783          	ld	a5,16(a0)
    80002478:	40b78733          	sub	a4,a5,a1
        if (remainingBlockCount > 0) {
    8000247c:	04b78c63          	beq	a5,a1,800024d4 <_ZN6kernel15MemoryAllocator14allocateBlocksEm+0xb0>
            size_t offset = blockCount * MEM_BLOCK_SIZE;
    80002480:	00659793          	slli	a5,a1,0x6
            auto newBlock = (FreeMemoryBlock*) ((char*) block + offset);
    80002484:	00f507b3          	add	a5,a0,a5

            if (block->prev) {
    80002488:	00053683          	ld	a3,0(a0)
    8000248c:	04068063          	beqz	a3,800024cc <_ZN6kernel15MemoryAllocator14allocateBlocksEm+0xa8>
                block->prev->next = newBlock;
    80002490:	00f6b423          	sd	a5,8(a3) # 8008 <offsetTCBregisters+0x7ff8>
            } else {
                freeMemoryHead = newBlock;
            }
            if(block->next){
    80002494:	00853683          	ld	a3,8(a0)
    80002498:	00068463          	beqz	a3,800024a0 <_ZN6kernel15MemoryAllocator14allocateBlocksEm+0x7c>
                block->next->prev = newBlock;
    8000249c:	00f6b023          	sd	a5,0(a3)
            }
            newBlock->prev = block->prev;
    800024a0:	00053683          	ld	a3,0(a0)
    800024a4:	00d7b023          	sd	a3,0(a5)
            newBlock->next = block->next;
    800024a8:	00853683          	ld	a3,8(a0)
    800024ac:	00d7b423          	sd	a3,8(a5)
            newBlock->size = remainingBlockCount;
    800024b0:	00e7b823          	sd	a4,16(a5)
            if(block->next) {
                block->next->prev = block->prev;
            }
        }

        block->next = nullptr;
    800024b4:	00053423          	sd	zero,8(a0)
        block->prev = nullptr;
        *((size_t*)block) = blockCount; // Upisi velicinu
    800024b8:	00b53023          	sd	a1,0(a0)

        return (char *) block + sizeof(size_t);
    800024bc:	00850513          	addi	a0,a0,8
    }
    800024c0:	00813403          	ld	s0,8(sp)
    800024c4:	01010113          	addi	sp,sp,16
    800024c8:	00008067          	ret
                freeMemoryHead = newBlock;
    800024cc:	00f63023          	sd	a5,0(a2)
    800024d0:	fc5ff06f          	j	80002494 <_ZN6kernel15MemoryAllocator14allocateBlocksEm+0x70>
            if (block->prev) {
    800024d4:	00053783          	ld	a5,0(a0)
    800024d8:	02078063          	beqz	a5,800024f8 <_ZN6kernel15MemoryAllocator14allocateBlocksEm+0xd4>
                block->prev->next = block->next;
    800024dc:	00853703          	ld	a4,8(a0)
    800024e0:	00e7b423          	sd	a4,8(a5)
            if(block->next) {
    800024e4:	00853783          	ld	a5,8(a0)
    800024e8:	fc0786e3          	beqz	a5,800024b4 <_ZN6kernel15MemoryAllocator14allocateBlocksEm+0x90>
                block->next->prev = block->prev;
    800024ec:	00053703          	ld	a4,0(a0)
    800024f0:	00e7b023          	sd	a4,0(a5)
    800024f4:	fc1ff06f          	j	800024b4 <_ZN6kernel15MemoryAllocator14allocateBlocksEm+0x90>
                freeMemoryHead = block->next;
    800024f8:	00853783          	ld	a5,8(a0)
    800024fc:	00f63023          	sd	a5,0(a2)
    80002500:	fe5ff06f          	j	800024e4 <_ZN6kernel15MemoryAllocator14allocateBlocksEm+0xc0>

0000000080002504 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv>:

    int MemoryAllocator::deallocateBlocks(void* address) {
    80002504:	ff010113          	addi	sp,sp,-16
    80002508:	00813423          	sd	s0,8(sp)
    8000250c:	01010413          	addi	s0,sp,16
        if(address == nullptr) return -1;
    80002510:	14058a63          	beqz	a1,80002664 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0x160>

        char* blockAddress = (char*)address - sizeof(size_t);
    80002514:	ff858693          	addi	a3,a1,-8

        // If address was not in heap range
        if(HEAP_START_ADDR > blockAddress || blockAddress >= HEAP_END_ADDR){
    80002518:	00004797          	auipc	a5,0x4
    8000251c:	ba87b783          	ld	a5,-1112(a5) # 800060c0 <_GLOBAL_OFFSET_TABLE_+0x8>
    80002520:	0007b783          	ld	a5,0(a5)
    80002524:	14f6e463          	bltu	a3,a5,8000266c <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0x168>
    80002528:	00004797          	auipc	a5,0x4
    8000252c:	bb07b783          	ld	a5,-1104(a5) # 800060d8 <_GLOBAL_OFFSET_TABLE_+0x20>
    80002530:	0007b783          	ld	a5,0(a5)
    80002534:	14f6f063          	bgeu	a3,a5,80002674 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0x170>
            return -2;
        }

        size_t blockCount = *((size_t*) blockAddress);
    80002538:	ff85b883          	ld	a7,-8(a1)
        size_t blocksSizeBytes = blockCount * MEM_BLOCK_SIZE;
    8000253c:	00689813          	slli	a6,a7,0x6

        // If end of address is after end of the heap
        if((char*)blockAddress + blocksSizeBytes >= HEAP_END_ADDR) {
    80002540:	01068833          	add	a6,a3,a6
    80002544:	12f87c63          	bgeu	a6,a5,8000267c <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0x178>
        }

        // Find the place where to insert new free segment
        // Insertion happens after the block pointed to by curr
        FreeMemoryBlock* curr = nullptr;
        if(!freeMemoryHead || blockAddress < (char*) freeMemoryHead){
    80002548:	00053603          	ld	a2,0(a0)
    8000254c:	06060863          	beqz	a2,800025bc <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0xb8>
    80002550:	06c6ea63          	bltu	a3,a2,800025c4 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0xc0>
            curr = nullptr; // Insert as the first
        }else{
            for(curr=freeMemoryHead; curr->next != nullptr && blockAddress > (char*)(curr->next); curr=curr->next);
    80002554:	00060793          	mv	a5,a2
    80002558:	00078713          	mv	a4,a5
    8000255c:	0087b783          	ld	a5,8(a5)
    80002560:	00078463          	beqz	a5,80002568 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0x64>
    80002564:	fed7eae3          	bltu	a5,a3,80002558 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0x54>
        }

        // Try to append block to the previous free segment curr
        if(curr && (char*)curr + curr->size * MEM_BLOCK_SIZE == blockAddress){
    80002568:	06070063          	beqz	a4,800025c8 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0xc4>
    8000256c:	01073e03          	ld	t3,16(a4)
    80002570:	006e1313          	slli	t1,t3,0x6
    80002574:	00670333          	add	t1,a4,t1
    80002578:	04d31863          	bne	t1,a3,800025c8 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0xc4>
            curr->size += blockCount;
    8000257c:	011e08b3          	add	a7,t3,a7
    80002580:	01173823          	sd	a7,16(a4)
            // Try to join curr with the next free address
            if(curr->next && (char*)curr+curr->size * MEM_BLOCK_SIZE  == (char*)(curr->next)){
    80002584:	00078863          	beqz	a5,80002594 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0x90>
    80002588:	00689693          	slli	a3,a7,0x6
    8000258c:	00d706b3          	add	a3,a4,a3
    80002590:	00d78663          	beq	a5,a3,8000259c <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0x98>
                curr->size += curr->next->size;
                curr->next = curr->next->next;
                if(curr->next) curr->next->prev = curr;
            }
            return 0;
    80002594:	00000513          	li	a0,0
    80002598:	06c0006f          	j	80002604 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0x100>
                curr->size += curr->next->size;
    8000259c:	0107b683          	ld	a3,16(a5)
    800025a0:	00d888b3          	add	a7,a7,a3
    800025a4:	01173823          	sd	a7,16(a4)
                curr->next = curr->next->next;
    800025a8:	0087b783          	ld	a5,8(a5)
    800025ac:	00f73423          	sd	a5,8(a4)
                if(curr->next) curr->next->prev = curr;
    800025b0:	fe0782e3          	beqz	a5,80002594 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0x90>
    800025b4:	00e7b023          	sd	a4,0(a5)
    800025b8:	fddff06f          	j	80002594 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0x90>
            curr = nullptr; // Insert as the first
    800025bc:	00060713          	mv	a4,a2
    800025c0:	0080006f          	j	800025c8 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0xc4>
    800025c4:	00000713          	li	a4,0
        }else {
            // Try to append it to the next free segment
            FreeMemoryBlock* nextSeg = curr?curr->next:freeMemoryHead;
    800025c8:	00070463          	beqz	a4,800025d0 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0xcc>
    800025cc:	00873603          	ld	a2,8(a4)
            if(nextSeg && (char*)blockAddress + blocksSizeBytes == (char*)nextSeg){
    800025d0:	00060463          	beqz	a2,800025d8 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0xd4>
    800025d4:	02c80e63          	beq	a6,a2,80002610 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0x10c>
            }
        }

        // No need to join; insert the new address after curr
        auto newSeg = (FreeMemoryBlock*) blockAddress;
        newSeg->size = blockCount;
    800025d8:	0115b423          	sd	a7,8(a1)
        newSeg->prev = curr;
    800025dc:	fee5bc23          	sd	a4,-8(a1)
        if(curr) {
    800025e0:	06070663          	beqz	a4,8000264c <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0x148>
            newSeg->next = curr->next;
    800025e4:	00873783          	ld	a5,8(a4)
    800025e8:	00f5b023          	sd	a5,0(a1)
        }else{
            newSeg->next = freeMemoryHead;
        }
        if(newSeg->next) {
    800025ec:	0005b783          	ld	a5,0(a1)
    800025f0:	00078463          	beqz	a5,800025f8 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0xf4>
            newSeg ->next->prev = newSeg;
    800025f4:	00d7b023          	sd	a3,0(a5)
        }
        if(curr) {
    800025f8:	06070063          	beqz	a4,80002658 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0x154>
            curr->next = newSeg;
    800025fc:	00d73423          	sd	a3,8(a4)
        }else{
            freeMemoryHead = newSeg;
        }

        return 0;
    80002600:	00000513          	li	a0,0
    }
    80002604:	00813403          	ld	s0,8(sp)
    80002608:	01010113          	addi	sp,sp,16
    8000260c:	00008067          	ret
                newSeg->size = blockCount + nextSeg->size;
    80002610:	01063783          	ld	a5,16(a2)
    80002614:	011788b3          	add	a7,a5,a7
    80002618:	0115b423          	sd	a7,8(a1)
                newSeg->prev = nextSeg->prev;
    8000261c:	00063783          	ld	a5,0(a2)
    80002620:	fef5bc23          	sd	a5,-8(a1)
                if(nextSeg->next){
    80002624:	00863783          	ld	a5,8(a2)
    80002628:	00078463          	beqz	a5,80002630 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0x12c>
                    nextSeg->next->prev = newSeg;
    8000262c:	00d7b023          	sd	a3,0(a5)
                if(nextSeg->prev) {
    80002630:	00063783          	ld	a5,0(a2)
    80002634:	00078863          	beqz	a5,80002644 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0x140>
                    nextSeg->prev->next = newSeg;
    80002638:	00d7b423          	sd	a3,8(a5)
                return 0;
    8000263c:	00000513          	li	a0,0
    80002640:	fc5ff06f          	j	80002604 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0x100>
                    freeMemoryHead = newSeg;
    80002644:	00d53023          	sd	a3,0(a0)
    80002648:	ff5ff06f          	j	8000263c <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0x138>
            newSeg->next = freeMemoryHead;
    8000264c:	00053783          	ld	a5,0(a0)
    80002650:	00f5b023          	sd	a5,0(a1)
    80002654:	f99ff06f          	j	800025ec <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0xe8>
            freeMemoryHead = newSeg;
    80002658:	00d53023          	sd	a3,0(a0)
        return 0;
    8000265c:	00000513          	li	a0,0
    80002660:	fa5ff06f          	j	80002604 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0x100>
        if(address == nullptr) return -1;
    80002664:	fff00513          	li	a0,-1
    80002668:	f9dff06f          	j	80002604 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0x100>
            return -2;
    8000266c:	ffe00513          	li	a0,-2
    80002670:	f95ff06f          	j	80002604 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0x100>
    80002674:	ffe00513          	li	a0,-2
    80002678:	f8dff06f          	j	80002604 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0x100>
            return  -3;
    8000267c:	ffd00513          	li	a0,-3
    80002680:	f85ff06f          	j	80002604 <_ZN6kernel15MemoryAllocator16deallocateBlocksEPv+0x100>

0000000080002684 <_ZN6kernel15MemoryAllocator13allocateBytesEm>:

    void *MemoryAllocator::allocateBytes(size_t byteCount) {
    80002684:	fe010113          	addi	sp,sp,-32
    80002688:	00113c23          	sd	ra,24(sp)
    8000268c:	00813823          	sd	s0,16(sp)
    80002690:	00913423          	sd	s1,8(sp)
    80002694:	02010413          	addi	s0,sp,32
    80002698:	00050493          	mv	s1,a0
        auto blockCount = byteSizeToBlockCount(byteCount);
    8000269c:	00058513          	mv	a0,a1
    800026a0:	00000097          	auipc	ra,0x0
    800026a4:	d64080e7          	jalr	-668(ra) # 80002404 <_ZN6kernel15MemoryAllocator20byteSizeToBlockCountEm>
    800026a8:	00050593          	mv	a1,a0
        return allocateBlocks(blockCount);
    800026ac:	00048513          	mv	a0,s1
    800026b0:	00000097          	auipc	ra,0x0
    800026b4:	d74080e7          	jalr	-652(ra) # 80002424 <_ZN6kernel15MemoryAllocator14allocateBlocksEm>
    }
    800026b8:	01813083          	ld	ra,24(sp)
    800026bc:	01013403          	ld	s0,16(sp)
    800026c0:	00813483          	ld	s1,8(sp)
    800026c4:	02010113          	addi	sp,sp,32
    800026c8:	00008067          	ret

00000000800026cc <_ZN6kernel9Collector7cleanupEv>:
// Created by os on 7/17/22.
//

#include "../../h/kernel/Collector.h"

void kernel::Collector::cleanup() {
    800026cc:	ff010113          	addi	sp,sp,-16
    800026d0:	00813423          	sd	s0,8(sp)
    800026d4:	01010413          	addi	s0,sp,16

}
    800026d8:	00813403          	ld	s0,8(sp)
    800026dc:	01010113          	addi	sp,sp,16
    800026e0:	00008067          	ret

00000000800026e4 <_Z16enableInterruptsv>:
#include "../h/kernel/TCB.h"
#include "../h/syscall_c.h"
#include "../lib/console.h"
#include "../h/kernel/ConsoleUtils.h"

void enableInterrupts() {
    800026e4:	ff010113          	addi	sp,sp,-16
    800026e8:	00813423          	sd	s0,8(sp)
    800026ec:	01010413          	addi	s0,sp,16
    using namespace kernel::BitMasks;
    SYS_REGISTER_SET_BITS(sstatus, sstatus::SIE);
    800026f0:	00200793          	li	a5,2
    800026f4:	1007a073          	csrs	sstatus,a5
}
    800026f8:	00813403          	ld	s0,8(sp)
    800026fc:	01010113          	addi	sp,sp,16
    80002700:	00008067          	ret

0000000080002704 <_Z17disableInterruptsv>:

void disableInterrupts() {
    80002704:	ff010113          	addi	sp,sp,-16
    80002708:	00813423          	sd	s0,8(sp)
    8000270c:	01010413          	addi	s0,sp,16
    using namespace kernel::BitMasks;
    SYS_REGISTER_CLEAR_BITS(sstatus, sstatus::SIE);
    80002710:	00200793          	li	a5,2
    80002714:	1007b073          	csrc	sstatus,a5
}
    80002718:	00813403          	ld	s0,8(sp)
    8000271c:	01010113          	addi	sp,sp,16
    80002720:	00008067          	ret

0000000080002724 <main>:

void main() {
    80002724:	ff010113          	addi	sp,sp,-16
    80002728:	00113423          	sd	ra,8(sp)
    8000272c:	00813023          	sd	s0,0(sp)
    80002730:	01010413          	addi	s0,sp,16
    using namespace kernel;
    // Set main trap handler
    WRITE_TO_SYS_REGISTER(stvec, &TrapHandlers::supervisorTrap);
    80002734:	00004797          	auipc	a5,0x4
    80002738:	9ac7b783          	ld	a5,-1620(a5) # 800060e0 <_GLOBAL_OFFSET_TABLE_+0x28>
    8000273c:	10579073          	csrw	stvec,a5

    //enableInterrupts();

    kernel::TCB::initialize();
    80002740:	00000097          	auipc	ra,0x0
    80002744:	b6c080e7          	jalr	-1172(ra) # 800022ac <_ZN6kernel3TCB10initializeEv>

    void userMain();
    userMain();
    80002748:	00000097          	auipc	ra,0x0
    8000274c:	2e8080e7          	jalr	744(ra) # 80002a30 <_Z8userMainv>
    while(Scheduler::getInstance().hasUserThreads()){
    80002750:	fffff097          	auipc	ra,0xfffff
    80002754:	450080e7          	jalr	1104(ra) # 80001ba0 <_ZN6kernel9Scheduler11getInstanceEv>
    80002758:	fffff097          	auipc	ra,0xfffff
    8000275c:	5e0080e7          	jalr	1504(ra) # 80001d38 <_ZNK6kernel9Scheduler14hasUserThreadsEv>
    80002760:	00050863          	beqz	a0,80002770 <main+0x4c>
        thread_dispatch();
    80002764:	fffff097          	auipc	ra,0xfffff
    80002768:	b14080e7          	jalr	-1260(ra) # 80001278 <_Z15thread_dispatchv>
    while(Scheduler::getInstance().hasUserThreads()){
    8000276c:	fe5ff06f          	j	80002750 <main+0x2c>
    }

    //disableInterrupts();

    kernel::Collector::cleanup();
    80002770:	00000097          	auipc	ra,0x0
    80002774:	f5c080e7          	jalr	-164(ra) # 800026cc <_ZN6kernel9Collector7cleanupEv>
}
    80002778:	00813083          	ld	ra,8(sp)
    8000277c:	00013403          	ld	s0,0(sp)
    80002780:	01010113          	addi	sp,sp,16
    80002784:	00008067          	ret

0000000080002788 <_ZZ8userMainvENUlPvE_4_FUNES_>:
    }
};

void userMain() {
    thread_t thread1;
    thread_create(&thread1, [](void *) {
    80002788:	fe010113          	addi	sp,sp,-32
    8000278c:	00113c23          	sd	ra,24(sp)
    80002790:	00813823          	sd	s0,16(sp)
    80002794:	00913423          	sd	s1,8(sp)
    80002798:	02010413          	addi	s0,sp,32
        for(int i = 0 ; i < 2; i++){
    8000279c:	00000493          	li	s1,0
    800027a0:	0200006f          	j	800027c0 <_ZZ8userMainvENUlPvE_4_FUNES_+0x38>
            printString("Hello from A!\n");
    800027a4:	00003517          	auipc	a0,0x3
    800027a8:	94c50513          	addi	a0,a0,-1716 # 800050f0 <CONSOLE_STATUS+0xe0>
    800027ac:	fffff097          	auipc	ra,0xfffff
    800027b0:	5ac080e7          	jalr	1452(ra) # 80001d58 <_Z11printStringPKc>
            thread_dispatch();
    800027b4:	fffff097          	auipc	ra,0xfffff
    800027b8:	ac4080e7          	jalr	-1340(ra) # 80001278 <_Z15thread_dispatchv>
        for(int i = 0 ; i < 2; i++){
    800027bc:	0014849b          	addiw	s1,s1,1
    800027c0:	00100793          	li	a5,1
    800027c4:	fe97d0e3          	bge	a5,s1,800027a4 <_ZZ8userMainvENUlPvE_4_FUNES_+0x1c>
        }
        thread_exit();
    800027c8:	fffff097          	auipc	ra,0xfffff
    800027cc:	a7c080e7          	jalr	-1412(ra) # 80001244 <_Z11thread_exitv>
    }, nullptr);
    800027d0:	01813083          	ld	ra,24(sp)
    800027d4:	01013403          	ld	s0,16(sp)
    800027d8:	00813483          	ld	s1,8(sp)
    800027dc:	02010113          	addi	sp,sp,32
    800027e0:	00008067          	ret

00000000800027e4 <_ZZ8userMainvENUlPvE0_4_FUNES_>:

    thread_t thread2;
    thread_create(&thread2, [](void *) {
    800027e4:	ff010113          	addi	sp,sp,-16
    800027e8:	00113423          	sd	ra,8(sp)
    800027ec:	00813023          	sd	s0,0(sp)
    800027f0:	01010413          	addi	s0,sp,16
        thread_exit();
    800027f4:	fffff097          	auipc	ra,0xfffff
    800027f8:	a50080e7          	jalr	-1456(ra) # 80001244 <_Z11thread_exitv>
        printString("Hello from B!\n");
    800027fc:	00003517          	auipc	a0,0x3
    80002800:	90450513          	addi	a0,a0,-1788 # 80005100 <CONSOLE_STATUS+0xf0>
    80002804:	fffff097          	auipc	ra,0xfffff
    80002808:	554080e7          	jalr	1364(ra) # 80001d58 <_Z11printStringPKc>
        thread_dispatch();
    8000280c:	fffff097          	auipc	ra,0xfffff
    80002810:	a6c080e7          	jalr	-1428(ra) # 80001278 <_Z15thread_dispatchv>
    }, nullptr);
    80002814:	00813083          	ld	ra,8(sp)
    80002818:	00013403          	ld	s0,0(sp)
    8000281c:	01010113          	addi	sp,sp,16
    80002820:	00008067          	ret

0000000080002824 <_ZZ8userMainvENUlPvE1_4_FUNES_>:

    thread_t thread3;
    thread_create(&thread3, [](void *) {
    80002824:	fe010113          	addi	sp,sp,-32
    80002828:	00113c23          	sd	ra,24(sp)
    8000282c:	00813823          	sd	s0,16(sp)
    80002830:	00913423          	sd	s1,8(sp)
    80002834:	02010413          	addi	s0,sp,32
        for(int i = 0; i < 3;i++){
    80002838:	00000493          	li	s1,0
    8000283c:	0280006f          	j	80002864 <_ZZ8userMainvENUlPvE1_4_FUNES_+0x40>
            if (i == 2) thread_exit();
    80002840:	fffff097          	auipc	ra,0xfffff
    80002844:	a04080e7          	jalr	-1532(ra) # 80001244 <_Z11thread_exitv>
            printString("Hello from C!\n");
    80002848:	00003517          	auipc	a0,0x3
    8000284c:	8c850513          	addi	a0,a0,-1848 # 80005110 <CONSOLE_STATUS+0x100>
    80002850:	fffff097          	auipc	ra,0xfffff
    80002854:	508080e7          	jalr	1288(ra) # 80001d58 <_Z11printStringPKc>
            thread_dispatch();
    80002858:	fffff097          	auipc	ra,0xfffff
    8000285c:	a20080e7          	jalr	-1504(ra) # 80001278 <_Z15thread_dispatchv>
        for(int i = 0; i < 3;i++){
    80002860:	0014849b          	addiw	s1,s1,1
    80002864:	00200793          	li	a5,2
    80002868:	0097c663          	blt	a5,s1,80002874 <_ZZ8userMainvENUlPvE1_4_FUNES_+0x50>
            if (i == 2) thread_exit();
    8000286c:	fcf49ee3          	bne	s1,a5,80002848 <_ZZ8userMainvENUlPvE1_4_FUNES_+0x24>
    80002870:	fd1ff06f          	j	80002840 <_ZZ8userMainvENUlPvE1_4_FUNES_+0x1c>
        }
    }, nullptr);
    80002874:	01813083          	ld	ra,24(sp)
    80002878:	01013403          	ld	s0,16(sp)
    8000287c:	00813483          	ld	s1,8(sp)
    80002880:	02010113          	addi	sp,sp,32
    80002884:	00008067          	ret

0000000080002888 <_ZZ8userMainvENUlPvE2_4_FUNES_>:

    SharedData data;

    thread_t threadA;
    thread_create(&threadA,[](void* arg){
    80002888:	fe010113          	addi	sp,sp,-32
    8000288c:	00113c23          	sd	ra,24(sp)
    80002890:	00813823          	sd	s0,16(sp)
    80002894:	00913423          	sd	s1,8(sp)
    80002898:	01213023          	sd	s2,0(sp)
    8000289c:	02010413          	addi	s0,sp,32
    800028a0:	00050493          	mv	s1,a0
        auto data = (SharedData*) arg;
        for(int i = 0; i < 3; i++) {
    800028a4:	00000913          	li	s2,0
    800028a8:	0340006f          	j	800028dc <_ZZ8userMainvENUlPvE2_4_FUNES_+0x54>
            sem_wait(data->sem);
    800028ac:	0084b503          	ld	a0,8(s1)
    800028b0:	fffff097          	auipc	ra,0xfffff
    800028b4:	a68080e7          	jalr	-1432(ra) # 80001318 <_Z8sem_waitP4_sem>
            data->arr[0] = i;
    800028b8:	0124a023          	sw	s2,0(s1)
            thread_dispatch();
    800028bc:	fffff097          	auipc	ra,0xfffff
    800028c0:	9bc080e7          	jalr	-1604(ra) # 80001278 <_Z15thread_dispatchv>
            sem_signal(data->sem);
    800028c4:	0084b503          	ld	a0,8(s1)
    800028c8:	fffff097          	auipc	ra,0xfffff
    800028cc:	a88080e7          	jalr	-1400(ra) # 80001350 <_Z10sem_signalP4_sem>
            thread_dispatch();
    800028d0:	fffff097          	auipc	ra,0xfffff
    800028d4:	9a8080e7          	jalr	-1624(ra) # 80001278 <_Z15thread_dispatchv>
        for(int i = 0; i < 3; i++) {
    800028d8:	0019091b          	addiw	s2,s2,1
    800028dc:	00200793          	li	a5,2
    800028e0:	fd27d6e3          	bge	a5,s2,800028ac <_ZZ8userMainvENUlPvE2_4_FUNES_+0x24>
        }
        data->done[0] = true;
    800028e4:	00100793          	li	a5,1
    800028e8:	00f48823          	sb	a5,16(s1)
    }, &data);
    800028ec:	01813083          	ld	ra,24(sp)
    800028f0:	01013403          	ld	s0,16(sp)
    800028f4:	00813483          	ld	s1,8(sp)
    800028f8:	00013903          	ld	s2,0(sp)
    800028fc:	02010113          	addi	sp,sp,32
    80002900:	00008067          	ret

0000000080002904 <_ZZ8userMainvENUlPvE3_4_FUNES_>:

    thread_t threadB;
    thread_create(&threadB,[](void* arg){
    80002904:	fe010113          	addi	sp,sp,-32
    80002908:	00113c23          	sd	ra,24(sp)
    8000290c:	00813823          	sd	s0,16(sp)
    80002910:	00913423          	sd	s1,8(sp)
    80002914:	01213023          	sd	s2,0(sp)
    80002918:	02010413          	addi	s0,sp,32
    8000291c:	00050493          	mv	s1,a0
        auto data = (SharedData*) arg;
        for(int i = 3; i < 6; i++){
    80002920:	00300913          	li	s2,3
    80002924:	0340006f          	j	80002958 <_ZZ8userMainvENUlPvE3_4_FUNES_+0x54>
            sem_wait(data->sem);
    80002928:	0084b503          	ld	a0,8(s1)
    8000292c:	fffff097          	auipc	ra,0xfffff
    80002930:	9ec080e7          	jalr	-1556(ra) # 80001318 <_Z8sem_waitP4_sem>
            data->arr[1] = i;
    80002934:	0124a223          	sw	s2,4(s1)
            thread_dispatch();
    80002938:	fffff097          	auipc	ra,0xfffff
    8000293c:	940080e7          	jalr	-1728(ra) # 80001278 <_Z15thread_dispatchv>
            sem_signal(data->sem);
    80002940:	0084b503          	ld	a0,8(s1)
    80002944:	fffff097          	auipc	ra,0xfffff
    80002948:	a0c080e7          	jalr	-1524(ra) # 80001350 <_Z10sem_signalP4_sem>
            thread_dispatch();
    8000294c:	fffff097          	auipc	ra,0xfffff
    80002950:	92c080e7          	jalr	-1748(ra) # 80001278 <_Z15thread_dispatchv>
        for(int i = 3; i < 6; i++){
    80002954:	0019091b          	addiw	s2,s2,1
    80002958:	00500793          	li	a5,5
    8000295c:	fd27d6e3          	bge	a5,s2,80002928 <_ZZ8userMainvENUlPvE3_4_FUNES_+0x24>
        }
        data->done[1] = true;
    80002960:	00100793          	li	a5,1
    80002964:	00f488a3          	sb	a5,17(s1)
    }, &data);
    80002968:	01813083          	ld	ra,24(sp)
    8000296c:	01013403          	ld	s0,16(sp)
    80002970:	00813483          	ld	s1,8(sp)
    80002974:	00013903          	ld	s2,0(sp)
    80002978:	02010113          	addi	sp,sp,32
    8000297c:	00008067          	ret

0000000080002980 <_ZZ8userMainvENUlPvE4_4_FUNES_>:

    thread_t threadC;
    thread_create(&threadC,[](void* arg){
    80002980:	fd010113          	addi	sp,sp,-48
    80002984:	02113423          	sd	ra,40(sp)
    80002988:	02813023          	sd	s0,32(sp)
    8000298c:	00913c23          	sd	s1,24(sp)
    80002990:	01213823          	sd	s2,16(sp)
    80002994:	03010413          	addi	s0,sp,48
    80002998:	00050493          	mv	s1,a0
        auto data = (SharedData*) arg;
        for(int i = 0; i < 3; i++){
    8000299c:	00000913          	li	s2,0
    800029a0:	0680006f          	j	80002a08 <_ZZ8userMainvENUlPvE4_4_FUNES_+0x88>
            sem_wait(data->sem);
    800029a4:	0084b503          	ld	a0,8(s1)
    800029a8:	fffff097          	auipc	ra,0xfffff
    800029ac:	970080e7          	jalr	-1680(ra) # 80001318 <_Z8sem_waitP4_sem>
            char str[5];
            str[0] = data->arr[0]+48;
    800029b0:	0004a783          	lw	a5,0(s1)
    800029b4:	0307879b          	addiw	a5,a5,48
    800029b8:	fcf40c23          	sb	a5,-40(s0)
            str[1] = ' ';
    800029bc:	02000793          	li	a5,32
    800029c0:	fcf40ca3          	sb	a5,-39(s0)
            thread_dispatch();
    800029c4:	fffff097          	auipc	ra,0xfffff
    800029c8:	8b4080e7          	jalr	-1868(ra) # 80001278 <_Z15thread_dispatchv>
            str[2] = data->arr[1]+48;
    800029cc:	0044a783          	lw	a5,4(s1)
    800029d0:	0307879b          	addiw	a5,a5,48
    800029d4:	fcf40d23          	sb	a5,-38(s0)
            str[3] = '\n';
    800029d8:	00a00793          	li	a5,10
    800029dc:	fcf40da3          	sb	a5,-37(s0)
            str[4] = '\0';
    800029e0:	fc040e23          	sb	zero,-36(s0)
            printString(str);
    800029e4:	fd840513          	addi	a0,s0,-40
    800029e8:	fffff097          	auipc	ra,0xfffff
    800029ec:	370080e7          	jalr	880(ra) # 80001d58 <_Z11printStringPKc>
            sem_signal(data->sem);
    800029f0:	0084b503          	ld	a0,8(s1)
    800029f4:	fffff097          	auipc	ra,0xfffff
    800029f8:	95c080e7          	jalr	-1700(ra) # 80001350 <_Z10sem_signalP4_sem>
            thread_dispatch();
    800029fc:	fffff097          	auipc	ra,0xfffff
    80002a00:	87c080e7          	jalr	-1924(ra) # 80001278 <_Z15thread_dispatchv>
        for(int i = 0; i < 3; i++){
    80002a04:	0019091b          	addiw	s2,s2,1
    80002a08:	00200793          	li	a5,2
    80002a0c:	f927dce3          	bge	a5,s2,800029a4 <_ZZ8userMainvENUlPvE4_4_FUNES_+0x24>
        }
        data->done[2] = true;
    80002a10:	00100793          	li	a5,1
    80002a14:	00f48923          	sb	a5,18(s1)
    }, &data);
    80002a18:	02813083          	ld	ra,40(sp)
    80002a1c:	02013403          	ld	s0,32(sp)
    80002a20:	01813483          	ld	s1,24(sp)
    80002a24:	01013903          	ld	s2,16(sp)
    80002a28:	03010113          	addi	sp,sp,48
    80002a2c:	00008067          	ret

0000000080002a30 <_Z8userMainv>:
void userMain() {
    80002a30:	f9010113          	addi	sp,sp,-112
    80002a34:	06113423          	sd	ra,104(sp)
    80002a38:	06813023          	sd	s0,96(sp)
    80002a3c:	04913c23          	sd	s1,88(sp)
    80002a40:	07010413          	addi	s0,sp,112
    thread_create(&thread1, [](void *) {
    80002a44:	00000613          	li	a2,0
    80002a48:	00000597          	auipc	a1,0x0
    80002a4c:	d4058593          	addi	a1,a1,-704 # 80002788 <_ZZ8userMainvENUlPvE_4_FUNES_>
    80002a50:	fd840513          	addi	a0,s0,-40
    80002a54:	ffffe097          	auipc	ra,0xffffe
    80002a58:	770080e7          	jalr	1904(ra) # 800011c4 <_Z13thread_createPP7_threadPFvPvES2_>
    thread_create(&thread2, [](void *) {
    80002a5c:	00000613          	li	a2,0
    80002a60:	00000597          	auipc	a1,0x0
    80002a64:	d8458593          	addi	a1,a1,-636 # 800027e4 <_ZZ8userMainvENUlPvE0_4_FUNES_>
    80002a68:	fd040513          	addi	a0,s0,-48
    80002a6c:	ffffe097          	auipc	ra,0xffffe
    80002a70:	758080e7          	jalr	1880(ra) # 800011c4 <_Z13thread_createPP7_threadPFvPvES2_>
    thread_create(&thread3, [](void *) {
    80002a74:	00000613          	li	a2,0
    80002a78:	00000597          	auipc	a1,0x0
    80002a7c:	dac58593          	addi	a1,a1,-596 # 80002824 <_ZZ8userMainvENUlPvE1_4_FUNES_>
    80002a80:	fc840513          	addi	a0,s0,-56
    80002a84:	ffffe097          	auipc	ra,0xffffe
    80002a88:	740080e7          	jalr	1856(ra) # 800011c4 <_Z13thread_createPP7_threadPFvPvES2_>
        sem_open(&sem,0);
    80002a8c:	00000593          	li	a1,0
    80002a90:	fb840513          	addi	a0,s0,-72
    80002a94:	fffff097          	auipc	ra,0xfffff
    80002a98:	810080e7          	jalr	-2032(ra) # 800012a4 <_Z8sem_openPP4_semj>
        arr[0] = arr[1] = 0;
    80002a9c:	fa042a23          	sw	zero,-76(s0)
    80002aa0:	fa042823          	sw	zero,-80(s0)
        for(int i = 0; i < 3; i++){
    80002aa4:	00000793          	li	a5,0
    80002aa8:	00200713          	li	a4,2
    80002aac:	00f74c63          	blt	a4,a5,80002ac4 <_Z8userMainv+0x94>
            done[i]=false;
    80002ab0:	fe040713          	addi	a4,s0,-32
    80002ab4:	00f70733          	add	a4,a4,a5
    80002ab8:	fe070023          	sb	zero,-32(a4)
        for(int i = 0; i < 3; i++){
    80002abc:	0017879b          	addiw	a5,a5,1
    80002ac0:	fe9ff06f          	j	80002aa8 <_Z8userMainv+0x78>
    thread_create(&threadA,[](void* arg){
    80002ac4:	fb040613          	addi	a2,s0,-80
    80002ac8:	00000597          	auipc	a1,0x0
    80002acc:	dc058593          	addi	a1,a1,-576 # 80002888 <_ZZ8userMainvENUlPvE2_4_FUNES_>
    80002ad0:	fa840513          	addi	a0,s0,-88
    80002ad4:	ffffe097          	auipc	ra,0xffffe
    80002ad8:	6f0080e7          	jalr	1776(ra) # 800011c4 <_Z13thread_createPP7_threadPFvPvES2_>
    thread_create(&threadB,[](void* arg){
    80002adc:	fb040613          	addi	a2,s0,-80
    80002ae0:	00000597          	auipc	a1,0x0
    80002ae4:	e2458593          	addi	a1,a1,-476 # 80002904 <_ZZ8userMainvENUlPvE3_4_FUNES_>
    80002ae8:	fa040513          	addi	a0,s0,-96
    80002aec:	ffffe097          	auipc	ra,0xffffe
    80002af0:	6d8080e7          	jalr	1752(ra) # 800011c4 <_Z13thread_createPP7_threadPFvPvES2_>
    thread_create(&threadC,[](void* arg){
    80002af4:	fb040613          	addi	a2,s0,-80
    80002af8:	00000597          	auipc	a1,0x0
    80002afc:	e8858593          	addi	a1,a1,-376 # 80002980 <_ZZ8userMainvENUlPvE4_4_FUNES_>
    80002b00:	f9840513          	addi	a0,s0,-104
    80002b04:	ffffe097          	auipc	ra,0xffffe
    80002b08:	6c0080e7          	jalr	1728(ra) # 800011c4 <_Z13thread_createPP7_threadPFvPvES2_>

    while(!data.done[0] && !data.done[1] && !data.done[2]){
    80002b0c:	fc044783          	lbu	a5,-64(s0)
    80002b10:	02079063          	bnez	a5,80002b30 <_Z8userMainv+0x100>
    80002b14:	fc144783          	lbu	a5,-63(s0)
    80002b18:	00079c63          	bnez	a5,80002b30 <_Z8userMainv+0x100>
    80002b1c:	fc244783          	lbu	a5,-62(s0)
    80002b20:	00079863          	bnez	a5,80002b30 <_Z8userMainv+0x100>
        thread_dispatch();
    80002b24:	ffffe097          	auipc	ra,0xffffe
    80002b28:	754080e7          	jalr	1876(ra) # 80001278 <_Z15thread_dispatchv>
    80002b2c:	fe1ff06f          	j	80002b0c <_Z8userMainv+0xdc>
        sem_close(sem);
    80002b30:	fb843503          	ld	a0,-72(s0)
    80002b34:	ffffe097          	auipc	ra,0xffffe
    80002b38:	7ac080e7          	jalr	1964(ra) # 800012e0 <_Z9sem_closeP4_sem>
    }
    80002b3c:	06813083          	ld	ra,104(sp)
    80002b40:	06013403          	ld	s0,96(sp)
    80002b44:	05813483          	ld	s1,88(sp)
    80002b48:	07010113          	addi	sp,sp,112
    80002b4c:	00008067          	ret
    80002b50:	00050493          	mv	s1,a0
        sem_close(sem);
    80002b54:	fb843503          	ld	a0,-72(s0)
    80002b58:	ffffe097          	auipc	ra,0xffffe
    80002b5c:	788080e7          	jalr	1928(ra) # 800012e0 <_Z9sem_closeP4_sem>
    80002b60:	00048513          	mv	a0,s1
    80002b64:	0000c097          	auipc	ra,0xc
    80002b68:	6e4080e7          	jalr	1764(ra) # 8000f248 <_Unwind_Resume>

0000000080002b6c <start>:
    80002b6c:	ff010113          	addi	sp,sp,-16
    80002b70:	00813423          	sd	s0,8(sp)
    80002b74:	01010413          	addi	s0,sp,16
    80002b78:	300027f3          	csrr	a5,mstatus
    80002b7c:	ffffe737          	lui	a4,0xffffe
    80002b80:	7ff70713          	addi	a4,a4,2047 # ffffffffffffe7ff <end+0xffffffff7ffef41f>
    80002b84:	00e7f7b3          	and	a5,a5,a4
    80002b88:	00001737          	lui	a4,0x1
    80002b8c:	80070713          	addi	a4,a4,-2048 # 800 <offsetTCBregisters+0x7f0>
    80002b90:	00e7e7b3          	or	a5,a5,a4
    80002b94:	30079073          	csrw	mstatus,a5
    80002b98:	00000797          	auipc	a5,0x0
    80002b9c:	16078793          	addi	a5,a5,352 # 80002cf8 <system_main>
    80002ba0:	34179073          	csrw	mepc,a5
    80002ba4:	00000793          	li	a5,0
    80002ba8:	18079073          	csrw	satp,a5
    80002bac:	000107b7          	lui	a5,0x10
    80002bb0:	fff78793          	addi	a5,a5,-1 # ffff <offsetTCBregisters+0xffef>
    80002bb4:	30279073          	csrw	medeleg,a5
    80002bb8:	30379073          	csrw	mideleg,a5
    80002bbc:	104027f3          	csrr	a5,sie
    80002bc0:	2227e793          	ori	a5,a5,546
    80002bc4:	10479073          	csrw	sie,a5
    80002bc8:	fff00793          	li	a5,-1
    80002bcc:	00a7d793          	srli	a5,a5,0xa
    80002bd0:	3b079073          	csrw	pmpaddr0,a5
    80002bd4:	00f00793          	li	a5,15
    80002bd8:	3a079073          	csrw	pmpcfg0,a5
    80002bdc:	f14027f3          	csrr	a5,mhartid
    80002be0:	0200c737          	lui	a4,0x200c
    80002be4:	ff873583          	ld	a1,-8(a4) # 200bff8 <offsetTCBregisters+0x200bfe8>
    80002be8:	0007869b          	sext.w	a3,a5
    80002bec:	00269713          	slli	a4,a3,0x2
    80002bf0:	000f4637          	lui	a2,0xf4
    80002bf4:	24060613          	addi	a2,a2,576 # f4240 <offsetTCBregisters+0xf4230>
    80002bf8:	00d70733          	add	a4,a4,a3
    80002bfc:	0037979b          	slliw	a5,a5,0x3
    80002c00:	020046b7          	lui	a3,0x2004
    80002c04:	00d787b3          	add	a5,a5,a3
    80002c08:	00c585b3          	add	a1,a1,a2
    80002c0c:	00371693          	slli	a3,a4,0x3
    80002c10:	0000b717          	auipc	a4,0xb
    80002c14:	57070713          	addi	a4,a4,1392 # 8000e180 <timer_scratch>
    80002c18:	00b7b023          	sd	a1,0(a5)
    80002c1c:	00d70733          	add	a4,a4,a3
    80002c20:	00f73c23          	sd	a5,24(a4)
    80002c24:	02c73023          	sd	a2,32(a4)
    80002c28:	34071073          	csrw	mscratch,a4
    80002c2c:	00000797          	auipc	a5,0x0
    80002c30:	6e478793          	addi	a5,a5,1764 # 80003310 <timervec>
    80002c34:	30579073          	csrw	mtvec,a5
    80002c38:	300027f3          	csrr	a5,mstatus
    80002c3c:	0087e793          	ori	a5,a5,8
    80002c40:	30079073          	csrw	mstatus,a5
    80002c44:	304027f3          	csrr	a5,mie
    80002c48:	0807e793          	ori	a5,a5,128
    80002c4c:	30479073          	csrw	mie,a5
    80002c50:	f14027f3          	csrr	a5,mhartid
    80002c54:	0007879b          	sext.w	a5,a5
    80002c58:	00078213          	mv	tp,a5
    80002c5c:	30200073          	mret
    80002c60:	00813403          	ld	s0,8(sp)
    80002c64:	01010113          	addi	sp,sp,16
    80002c68:	00008067          	ret

0000000080002c6c <timerinit>:
    80002c6c:	ff010113          	addi	sp,sp,-16
    80002c70:	00813423          	sd	s0,8(sp)
    80002c74:	01010413          	addi	s0,sp,16
    80002c78:	f14027f3          	csrr	a5,mhartid
    80002c7c:	0200c737          	lui	a4,0x200c
    80002c80:	ff873583          	ld	a1,-8(a4) # 200bff8 <offsetTCBregisters+0x200bfe8>
    80002c84:	0007869b          	sext.w	a3,a5
    80002c88:	00269713          	slli	a4,a3,0x2
    80002c8c:	000f4637          	lui	a2,0xf4
    80002c90:	24060613          	addi	a2,a2,576 # f4240 <offsetTCBregisters+0xf4230>
    80002c94:	00d70733          	add	a4,a4,a3
    80002c98:	0037979b          	slliw	a5,a5,0x3
    80002c9c:	020046b7          	lui	a3,0x2004
    80002ca0:	00d787b3          	add	a5,a5,a3
    80002ca4:	00c585b3          	add	a1,a1,a2
    80002ca8:	00371693          	slli	a3,a4,0x3
    80002cac:	0000b717          	auipc	a4,0xb
    80002cb0:	4d470713          	addi	a4,a4,1236 # 8000e180 <timer_scratch>
    80002cb4:	00b7b023          	sd	a1,0(a5)
    80002cb8:	00d70733          	add	a4,a4,a3
    80002cbc:	00f73c23          	sd	a5,24(a4)
    80002cc0:	02c73023          	sd	a2,32(a4)
    80002cc4:	34071073          	csrw	mscratch,a4
    80002cc8:	00000797          	auipc	a5,0x0
    80002ccc:	64878793          	addi	a5,a5,1608 # 80003310 <timervec>
    80002cd0:	30579073          	csrw	mtvec,a5
    80002cd4:	300027f3          	csrr	a5,mstatus
    80002cd8:	0087e793          	ori	a5,a5,8
    80002cdc:	30079073          	csrw	mstatus,a5
    80002ce0:	304027f3          	csrr	a5,mie
    80002ce4:	0807e793          	ori	a5,a5,128
    80002ce8:	30479073          	csrw	mie,a5
    80002cec:	00813403          	ld	s0,8(sp)
    80002cf0:	01010113          	addi	sp,sp,16
    80002cf4:	00008067          	ret

0000000080002cf8 <system_main>:
    80002cf8:	fe010113          	addi	sp,sp,-32
    80002cfc:	00813823          	sd	s0,16(sp)
    80002d00:	00913423          	sd	s1,8(sp)
    80002d04:	00113c23          	sd	ra,24(sp)
    80002d08:	02010413          	addi	s0,sp,32
    80002d0c:	00000097          	auipc	ra,0x0
    80002d10:	0c4080e7          	jalr	196(ra) # 80002dd0 <cpuid>
    80002d14:	00003497          	auipc	s1,0x3
    80002d18:	3ec48493          	addi	s1,s1,1004 # 80006100 <started>
    80002d1c:	02050263          	beqz	a0,80002d40 <system_main+0x48>
    80002d20:	0004a783          	lw	a5,0(s1)
    80002d24:	0007879b          	sext.w	a5,a5
    80002d28:	fe078ce3          	beqz	a5,80002d20 <system_main+0x28>
    80002d2c:	0ff0000f          	fence
    80002d30:	00002517          	auipc	a0,0x2
    80002d34:	42050513          	addi	a0,a0,1056 # 80005150 <CONSOLE_STATUS+0x140>
    80002d38:	00001097          	auipc	ra,0x1
    80002d3c:	a74080e7          	jalr	-1420(ra) # 800037ac <panic>
    80002d40:	00001097          	auipc	ra,0x1
    80002d44:	9c8080e7          	jalr	-1592(ra) # 80003708 <consoleinit>
    80002d48:	00001097          	auipc	ra,0x1
    80002d4c:	154080e7          	jalr	340(ra) # 80003e9c <printfinit>
    80002d50:	00002517          	auipc	a0,0x2
    80002d54:	4e050513          	addi	a0,a0,1248 # 80005230 <CONSOLE_STATUS+0x220>
    80002d58:	00001097          	auipc	ra,0x1
    80002d5c:	ab0080e7          	jalr	-1360(ra) # 80003808 <__printf>
    80002d60:	00002517          	auipc	a0,0x2
    80002d64:	3c050513          	addi	a0,a0,960 # 80005120 <CONSOLE_STATUS+0x110>
    80002d68:	00001097          	auipc	ra,0x1
    80002d6c:	aa0080e7          	jalr	-1376(ra) # 80003808 <__printf>
    80002d70:	00002517          	auipc	a0,0x2
    80002d74:	4c050513          	addi	a0,a0,1216 # 80005230 <CONSOLE_STATUS+0x220>
    80002d78:	00001097          	auipc	ra,0x1
    80002d7c:	a90080e7          	jalr	-1392(ra) # 80003808 <__printf>
    80002d80:	00001097          	auipc	ra,0x1
    80002d84:	4a8080e7          	jalr	1192(ra) # 80004228 <kinit>
    80002d88:	00000097          	auipc	ra,0x0
    80002d8c:	148080e7          	jalr	328(ra) # 80002ed0 <trapinit>
    80002d90:	00000097          	auipc	ra,0x0
    80002d94:	16c080e7          	jalr	364(ra) # 80002efc <trapinithart>
    80002d98:	00000097          	auipc	ra,0x0
    80002d9c:	5b8080e7          	jalr	1464(ra) # 80003350 <plicinit>
    80002da0:	00000097          	auipc	ra,0x0
    80002da4:	5d8080e7          	jalr	1496(ra) # 80003378 <plicinithart>
    80002da8:	00000097          	auipc	ra,0x0
    80002dac:	078080e7          	jalr	120(ra) # 80002e20 <userinit>
    80002db0:	0ff0000f          	fence
    80002db4:	00100793          	li	a5,1
    80002db8:	00002517          	auipc	a0,0x2
    80002dbc:	38050513          	addi	a0,a0,896 # 80005138 <CONSOLE_STATUS+0x128>
    80002dc0:	00f4a023          	sw	a5,0(s1)
    80002dc4:	00001097          	auipc	ra,0x1
    80002dc8:	a44080e7          	jalr	-1468(ra) # 80003808 <__printf>
    80002dcc:	0000006f          	j	80002dcc <system_main+0xd4>

0000000080002dd0 <cpuid>:
    80002dd0:	ff010113          	addi	sp,sp,-16
    80002dd4:	00813423          	sd	s0,8(sp)
    80002dd8:	01010413          	addi	s0,sp,16
    80002ddc:	00020513          	mv	a0,tp
    80002de0:	00813403          	ld	s0,8(sp)
    80002de4:	0005051b          	sext.w	a0,a0
    80002de8:	01010113          	addi	sp,sp,16
    80002dec:	00008067          	ret

0000000080002df0 <mycpu>:
    80002df0:	ff010113          	addi	sp,sp,-16
    80002df4:	00813423          	sd	s0,8(sp)
    80002df8:	01010413          	addi	s0,sp,16
    80002dfc:	00020793          	mv	a5,tp
    80002e00:	00813403          	ld	s0,8(sp)
    80002e04:	0007879b          	sext.w	a5,a5
    80002e08:	00779793          	slli	a5,a5,0x7
    80002e0c:	0000c517          	auipc	a0,0xc
    80002e10:	3a450513          	addi	a0,a0,932 # 8000f1b0 <cpus>
    80002e14:	00f50533          	add	a0,a0,a5
    80002e18:	01010113          	addi	sp,sp,16
    80002e1c:	00008067          	ret

0000000080002e20 <userinit>:
    80002e20:	ff010113          	addi	sp,sp,-16
    80002e24:	00813423          	sd	s0,8(sp)
    80002e28:	01010413          	addi	s0,sp,16
    80002e2c:	00813403          	ld	s0,8(sp)
    80002e30:	01010113          	addi	sp,sp,16
    80002e34:	00000317          	auipc	t1,0x0
    80002e38:	8f030067          	jr	-1808(t1) # 80002724 <main>

0000000080002e3c <either_copyout>:
    80002e3c:	ff010113          	addi	sp,sp,-16
    80002e40:	00813023          	sd	s0,0(sp)
    80002e44:	00113423          	sd	ra,8(sp)
    80002e48:	01010413          	addi	s0,sp,16
    80002e4c:	02051663          	bnez	a0,80002e78 <either_copyout+0x3c>
    80002e50:	00058513          	mv	a0,a1
    80002e54:	00060593          	mv	a1,a2
    80002e58:	0006861b          	sext.w	a2,a3
    80002e5c:	00002097          	auipc	ra,0x2
    80002e60:	c58080e7          	jalr	-936(ra) # 80004ab4 <__memmove>
    80002e64:	00813083          	ld	ra,8(sp)
    80002e68:	00013403          	ld	s0,0(sp)
    80002e6c:	00000513          	li	a0,0
    80002e70:	01010113          	addi	sp,sp,16
    80002e74:	00008067          	ret
    80002e78:	00002517          	auipc	a0,0x2
    80002e7c:	30050513          	addi	a0,a0,768 # 80005178 <CONSOLE_STATUS+0x168>
    80002e80:	00001097          	auipc	ra,0x1
    80002e84:	92c080e7          	jalr	-1748(ra) # 800037ac <panic>

0000000080002e88 <either_copyin>:
    80002e88:	ff010113          	addi	sp,sp,-16
    80002e8c:	00813023          	sd	s0,0(sp)
    80002e90:	00113423          	sd	ra,8(sp)
    80002e94:	01010413          	addi	s0,sp,16
    80002e98:	02059463          	bnez	a1,80002ec0 <either_copyin+0x38>
    80002e9c:	00060593          	mv	a1,a2
    80002ea0:	0006861b          	sext.w	a2,a3
    80002ea4:	00002097          	auipc	ra,0x2
    80002ea8:	c10080e7          	jalr	-1008(ra) # 80004ab4 <__memmove>
    80002eac:	00813083          	ld	ra,8(sp)
    80002eb0:	00013403          	ld	s0,0(sp)
    80002eb4:	00000513          	li	a0,0
    80002eb8:	01010113          	addi	sp,sp,16
    80002ebc:	00008067          	ret
    80002ec0:	00002517          	auipc	a0,0x2
    80002ec4:	2e050513          	addi	a0,a0,736 # 800051a0 <CONSOLE_STATUS+0x190>
    80002ec8:	00001097          	auipc	ra,0x1
    80002ecc:	8e4080e7          	jalr	-1820(ra) # 800037ac <panic>

0000000080002ed0 <trapinit>:
    80002ed0:	ff010113          	addi	sp,sp,-16
    80002ed4:	00813423          	sd	s0,8(sp)
    80002ed8:	01010413          	addi	s0,sp,16
    80002edc:	00813403          	ld	s0,8(sp)
    80002ee0:	00002597          	auipc	a1,0x2
    80002ee4:	2e858593          	addi	a1,a1,744 # 800051c8 <CONSOLE_STATUS+0x1b8>
    80002ee8:	0000c517          	auipc	a0,0xc
    80002eec:	34850513          	addi	a0,a0,840 # 8000f230 <tickslock>
    80002ef0:	01010113          	addi	sp,sp,16
    80002ef4:	00001317          	auipc	t1,0x1
    80002ef8:	5c430067          	jr	1476(t1) # 800044b8 <initlock>

0000000080002efc <trapinithart>:
    80002efc:	ff010113          	addi	sp,sp,-16
    80002f00:	00813423          	sd	s0,8(sp)
    80002f04:	01010413          	addi	s0,sp,16
    80002f08:	00000797          	auipc	a5,0x0
    80002f0c:	2f878793          	addi	a5,a5,760 # 80003200 <kernelvec>
    80002f10:	10579073          	csrw	stvec,a5
    80002f14:	00813403          	ld	s0,8(sp)
    80002f18:	01010113          	addi	sp,sp,16
    80002f1c:	00008067          	ret

0000000080002f20 <usertrap>:
    80002f20:	ff010113          	addi	sp,sp,-16
    80002f24:	00813423          	sd	s0,8(sp)
    80002f28:	01010413          	addi	s0,sp,16
    80002f2c:	00813403          	ld	s0,8(sp)
    80002f30:	01010113          	addi	sp,sp,16
    80002f34:	00008067          	ret

0000000080002f38 <usertrapret>:
    80002f38:	ff010113          	addi	sp,sp,-16
    80002f3c:	00813423          	sd	s0,8(sp)
    80002f40:	01010413          	addi	s0,sp,16
    80002f44:	00813403          	ld	s0,8(sp)
    80002f48:	01010113          	addi	sp,sp,16
    80002f4c:	00008067          	ret

0000000080002f50 <kerneltrap>:
    80002f50:	fe010113          	addi	sp,sp,-32
    80002f54:	00813823          	sd	s0,16(sp)
    80002f58:	00113c23          	sd	ra,24(sp)
    80002f5c:	00913423          	sd	s1,8(sp)
    80002f60:	02010413          	addi	s0,sp,32
    80002f64:	142025f3          	csrr	a1,scause
    80002f68:	100027f3          	csrr	a5,sstatus
    80002f6c:	0027f793          	andi	a5,a5,2
    80002f70:	10079c63          	bnez	a5,80003088 <kerneltrap+0x138>
    80002f74:	142027f3          	csrr	a5,scause
    80002f78:	0207ce63          	bltz	a5,80002fb4 <kerneltrap+0x64>
    80002f7c:	00002517          	auipc	a0,0x2
    80002f80:	29450513          	addi	a0,a0,660 # 80005210 <CONSOLE_STATUS+0x200>
    80002f84:	00001097          	auipc	ra,0x1
    80002f88:	884080e7          	jalr	-1916(ra) # 80003808 <__printf>
    80002f8c:	141025f3          	csrr	a1,sepc
    80002f90:	14302673          	csrr	a2,stval
    80002f94:	00002517          	auipc	a0,0x2
    80002f98:	28c50513          	addi	a0,a0,652 # 80005220 <CONSOLE_STATUS+0x210>
    80002f9c:	00001097          	auipc	ra,0x1
    80002fa0:	86c080e7          	jalr	-1940(ra) # 80003808 <__printf>
    80002fa4:	00002517          	auipc	a0,0x2
    80002fa8:	29450513          	addi	a0,a0,660 # 80005238 <CONSOLE_STATUS+0x228>
    80002fac:	00001097          	auipc	ra,0x1
    80002fb0:	800080e7          	jalr	-2048(ra) # 800037ac <panic>
    80002fb4:	0ff7f713          	andi	a4,a5,255
    80002fb8:	00900693          	li	a3,9
    80002fbc:	04d70063          	beq	a4,a3,80002ffc <kerneltrap+0xac>
    80002fc0:	fff00713          	li	a4,-1
    80002fc4:	03f71713          	slli	a4,a4,0x3f
    80002fc8:	00170713          	addi	a4,a4,1
    80002fcc:	fae798e3          	bne	a5,a4,80002f7c <kerneltrap+0x2c>
    80002fd0:	00000097          	auipc	ra,0x0
    80002fd4:	e00080e7          	jalr	-512(ra) # 80002dd0 <cpuid>
    80002fd8:	06050663          	beqz	a0,80003044 <kerneltrap+0xf4>
    80002fdc:	144027f3          	csrr	a5,sip
    80002fe0:	ffd7f793          	andi	a5,a5,-3
    80002fe4:	14479073          	csrw	sip,a5
    80002fe8:	01813083          	ld	ra,24(sp)
    80002fec:	01013403          	ld	s0,16(sp)
    80002ff0:	00813483          	ld	s1,8(sp)
    80002ff4:	02010113          	addi	sp,sp,32
    80002ff8:	00008067          	ret
    80002ffc:	00000097          	auipc	ra,0x0
    80003000:	3c8080e7          	jalr	968(ra) # 800033c4 <plic_claim>
    80003004:	00a00793          	li	a5,10
    80003008:	00050493          	mv	s1,a0
    8000300c:	06f50863          	beq	a0,a5,8000307c <kerneltrap+0x12c>
    80003010:	fc050ce3          	beqz	a0,80002fe8 <kerneltrap+0x98>
    80003014:	00050593          	mv	a1,a0
    80003018:	00002517          	auipc	a0,0x2
    8000301c:	1d850513          	addi	a0,a0,472 # 800051f0 <CONSOLE_STATUS+0x1e0>
    80003020:	00000097          	auipc	ra,0x0
    80003024:	7e8080e7          	jalr	2024(ra) # 80003808 <__printf>
    80003028:	01013403          	ld	s0,16(sp)
    8000302c:	01813083          	ld	ra,24(sp)
    80003030:	00048513          	mv	a0,s1
    80003034:	00813483          	ld	s1,8(sp)
    80003038:	02010113          	addi	sp,sp,32
    8000303c:	00000317          	auipc	t1,0x0
    80003040:	3c030067          	jr	960(t1) # 800033fc <plic_complete>
    80003044:	0000c517          	auipc	a0,0xc
    80003048:	1ec50513          	addi	a0,a0,492 # 8000f230 <tickslock>
    8000304c:	00001097          	auipc	ra,0x1
    80003050:	490080e7          	jalr	1168(ra) # 800044dc <acquire>
    80003054:	00003717          	auipc	a4,0x3
    80003058:	0b070713          	addi	a4,a4,176 # 80006104 <ticks>
    8000305c:	00072783          	lw	a5,0(a4)
    80003060:	0000c517          	auipc	a0,0xc
    80003064:	1d050513          	addi	a0,a0,464 # 8000f230 <tickslock>
    80003068:	0017879b          	addiw	a5,a5,1
    8000306c:	00f72023          	sw	a5,0(a4)
    80003070:	00001097          	auipc	ra,0x1
    80003074:	538080e7          	jalr	1336(ra) # 800045a8 <release>
    80003078:	f65ff06f          	j	80002fdc <kerneltrap+0x8c>
    8000307c:	00001097          	auipc	ra,0x1
    80003080:	094080e7          	jalr	148(ra) # 80004110 <uartintr>
    80003084:	fa5ff06f          	j	80003028 <kerneltrap+0xd8>
    80003088:	00002517          	auipc	a0,0x2
    8000308c:	14850513          	addi	a0,a0,328 # 800051d0 <CONSOLE_STATUS+0x1c0>
    80003090:	00000097          	auipc	ra,0x0
    80003094:	71c080e7          	jalr	1820(ra) # 800037ac <panic>

0000000080003098 <clockintr>:
    80003098:	fe010113          	addi	sp,sp,-32
    8000309c:	00813823          	sd	s0,16(sp)
    800030a0:	00913423          	sd	s1,8(sp)
    800030a4:	00113c23          	sd	ra,24(sp)
    800030a8:	02010413          	addi	s0,sp,32
    800030ac:	0000c497          	auipc	s1,0xc
    800030b0:	18448493          	addi	s1,s1,388 # 8000f230 <tickslock>
    800030b4:	00048513          	mv	a0,s1
    800030b8:	00001097          	auipc	ra,0x1
    800030bc:	424080e7          	jalr	1060(ra) # 800044dc <acquire>
    800030c0:	00003717          	auipc	a4,0x3
    800030c4:	04470713          	addi	a4,a4,68 # 80006104 <ticks>
    800030c8:	00072783          	lw	a5,0(a4)
    800030cc:	01013403          	ld	s0,16(sp)
    800030d0:	01813083          	ld	ra,24(sp)
    800030d4:	00048513          	mv	a0,s1
    800030d8:	0017879b          	addiw	a5,a5,1
    800030dc:	00813483          	ld	s1,8(sp)
    800030e0:	00f72023          	sw	a5,0(a4)
    800030e4:	02010113          	addi	sp,sp,32
    800030e8:	00001317          	auipc	t1,0x1
    800030ec:	4c030067          	jr	1216(t1) # 800045a8 <release>

00000000800030f0 <devintr>:
    800030f0:	142027f3          	csrr	a5,scause
    800030f4:	00000513          	li	a0,0
    800030f8:	0007c463          	bltz	a5,80003100 <devintr+0x10>
    800030fc:	00008067          	ret
    80003100:	fe010113          	addi	sp,sp,-32
    80003104:	00813823          	sd	s0,16(sp)
    80003108:	00113c23          	sd	ra,24(sp)
    8000310c:	00913423          	sd	s1,8(sp)
    80003110:	02010413          	addi	s0,sp,32
    80003114:	0ff7f713          	andi	a4,a5,255
    80003118:	00900693          	li	a3,9
    8000311c:	04d70c63          	beq	a4,a3,80003174 <devintr+0x84>
    80003120:	fff00713          	li	a4,-1
    80003124:	03f71713          	slli	a4,a4,0x3f
    80003128:	00170713          	addi	a4,a4,1
    8000312c:	00e78c63          	beq	a5,a4,80003144 <devintr+0x54>
    80003130:	01813083          	ld	ra,24(sp)
    80003134:	01013403          	ld	s0,16(sp)
    80003138:	00813483          	ld	s1,8(sp)
    8000313c:	02010113          	addi	sp,sp,32
    80003140:	00008067          	ret
    80003144:	00000097          	auipc	ra,0x0
    80003148:	c8c080e7          	jalr	-884(ra) # 80002dd0 <cpuid>
    8000314c:	06050663          	beqz	a0,800031b8 <devintr+0xc8>
    80003150:	144027f3          	csrr	a5,sip
    80003154:	ffd7f793          	andi	a5,a5,-3
    80003158:	14479073          	csrw	sip,a5
    8000315c:	01813083          	ld	ra,24(sp)
    80003160:	01013403          	ld	s0,16(sp)
    80003164:	00813483          	ld	s1,8(sp)
    80003168:	00200513          	li	a0,2
    8000316c:	02010113          	addi	sp,sp,32
    80003170:	00008067          	ret
    80003174:	00000097          	auipc	ra,0x0
    80003178:	250080e7          	jalr	592(ra) # 800033c4 <plic_claim>
    8000317c:	00a00793          	li	a5,10
    80003180:	00050493          	mv	s1,a0
    80003184:	06f50663          	beq	a0,a5,800031f0 <devintr+0x100>
    80003188:	00100513          	li	a0,1
    8000318c:	fa0482e3          	beqz	s1,80003130 <devintr+0x40>
    80003190:	00048593          	mv	a1,s1
    80003194:	00002517          	auipc	a0,0x2
    80003198:	05c50513          	addi	a0,a0,92 # 800051f0 <CONSOLE_STATUS+0x1e0>
    8000319c:	00000097          	auipc	ra,0x0
    800031a0:	66c080e7          	jalr	1644(ra) # 80003808 <__printf>
    800031a4:	00048513          	mv	a0,s1
    800031a8:	00000097          	auipc	ra,0x0
    800031ac:	254080e7          	jalr	596(ra) # 800033fc <plic_complete>
    800031b0:	00100513          	li	a0,1
    800031b4:	f7dff06f          	j	80003130 <devintr+0x40>
    800031b8:	0000c517          	auipc	a0,0xc
    800031bc:	07850513          	addi	a0,a0,120 # 8000f230 <tickslock>
    800031c0:	00001097          	auipc	ra,0x1
    800031c4:	31c080e7          	jalr	796(ra) # 800044dc <acquire>
    800031c8:	00003717          	auipc	a4,0x3
    800031cc:	f3c70713          	addi	a4,a4,-196 # 80006104 <ticks>
    800031d0:	00072783          	lw	a5,0(a4)
    800031d4:	0000c517          	auipc	a0,0xc
    800031d8:	05c50513          	addi	a0,a0,92 # 8000f230 <tickslock>
    800031dc:	0017879b          	addiw	a5,a5,1
    800031e0:	00f72023          	sw	a5,0(a4)
    800031e4:	00001097          	auipc	ra,0x1
    800031e8:	3c4080e7          	jalr	964(ra) # 800045a8 <release>
    800031ec:	f65ff06f          	j	80003150 <devintr+0x60>
    800031f0:	00001097          	auipc	ra,0x1
    800031f4:	f20080e7          	jalr	-224(ra) # 80004110 <uartintr>
    800031f8:	fadff06f          	j	800031a4 <devintr+0xb4>
    800031fc:	0000                	unimp
	...

0000000080003200 <kernelvec>:
    80003200:	f0010113          	addi	sp,sp,-256
    80003204:	00113023          	sd	ra,0(sp)
    80003208:	00213423          	sd	sp,8(sp)
    8000320c:	00313823          	sd	gp,16(sp)
    80003210:	00413c23          	sd	tp,24(sp)
    80003214:	02513023          	sd	t0,32(sp)
    80003218:	02613423          	sd	t1,40(sp)
    8000321c:	02713823          	sd	t2,48(sp)
    80003220:	02813c23          	sd	s0,56(sp)
    80003224:	04913023          	sd	s1,64(sp)
    80003228:	04a13423          	sd	a0,72(sp)
    8000322c:	04b13823          	sd	a1,80(sp)
    80003230:	04c13c23          	sd	a2,88(sp)
    80003234:	06d13023          	sd	a3,96(sp)
    80003238:	06e13423          	sd	a4,104(sp)
    8000323c:	06f13823          	sd	a5,112(sp)
    80003240:	07013c23          	sd	a6,120(sp)
    80003244:	09113023          	sd	a7,128(sp)
    80003248:	09213423          	sd	s2,136(sp)
    8000324c:	09313823          	sd	s3,144(sp)
    80003250:	09413c23          	sd	s4,152(sp)
    80003254:	0b513023          	sd	s5,160(sp)
    80003258:	0b613423          	sd	s6,168(sp)
    8000325c:	0b713823          	sd	s7,176(sp)
    80003260:	0b813c23          	sd	s8,184(sp)
    80003264:	0d913023          	sd	s9,192(sp)
    80003268:	0da13423          	sd	s10,200(sp)
    8000326c:	0db13823          	sd	s11,208(sp)
    80003270:	0dc13c23          	sd	t3,216(sp)
    80003274:	0fd13023          	sd	t4,224(sp)
    80003278:	0fe13423          	sd	t5,232(sp)
    8000327c:	0ff13823          	sd	t6,240(sp)
    80003280:	cd1ff0ef          	jal	ra,80002f50 <kerneltrap>
    80003284:	00013083          	ld	ra,0(sp)
    80003288:	00813103          	ld	sp,8(sp)
    8000328c:	01013183          	ld	gp,16(sp)
    80003290:	02013283          	ld	t0,32(sp)
    80003294:	02813303          	ld	t1,40(sp)
    80003298:	03013383          	ld	t2,48(sp)
    8000329c:	03813403          	ld	s0,56(sp)
    800032a0:	04013483          	ld	s1,64(sp)
    800032a4:	04813503          	ld	a0,72(sp)
    800032a8:	05013583          	ld	a1,80(sp)
    800032ac:	05813603          	ld	a2,88(sp)
    800032b0:	06013683          	ld	a3,96(sp)
    800032b4:	06813703          	ld	a4,104(sp)
    800032b8:	07013783          	ld	a5,112(sp)
    800032bc:	07813803          	ld	a6,120(sp)
    800032c0:	08013883          	ld	a7,128(sp)
    800032c4:	08813903          	ld	s2,136(sp)
    800032c8:	09013983          	ld	s3,144(sp)
    800032cc:	09813a03          	ld	s4,152(sp)
    800032d0:	0a013a83          	ld	s5,160(sp)
    800032d4:	0a813b03          	ld	s6,168(sp)
    800032d8:	0b013b83          	ld	s7,176(sp)
    800032dc:	0b813c03          	ld	s8,184(sp)
    800032e0:	0c013c83          	ld	s9,192(sp)
    800032e4:	0c813d03          	ld	s10,200(sp)
    800032e8:	0d013d83          	ld	s11,208(sp)
    800032ec:	0d813e03          	ld	t3,216(sp)
    800032f0:	0e013e83          	ld	t4,224(sp)
    800032f4:	0e813f03          	ld	t5,232(sp)
    800032f8:	0f013f83          	ld	t6,240(sp)
    800032fc:	10010113          	addi	sp,sp,256
    80003300:	10200073          	sret
    80003304:	00000013          	nop
    80003308:	00000013          	nop
    8000330c:	00000013          	nop

0000000080003310 <timervec>:
    80003310:	34051573          	csrrw	a0,mscratch,a0
    80003314:	00b53023          	sd	a1,0(a0)
    80003318:	00c53423          	sd	a2,8(a0)
    8000331c:	00d53823          	sd	a3,16(a0)
    80003320:	01853583          	ld	a1,24(a0)
    80003324:	02053603          	ld	a2,32(a0)
    80003328:	0005b683          	ld	a3,0(a1)
    8000332c:	00c686b3          	add	a3,a3,a2
    80003330:	00d5b023          	sd	a3,0(a1)
    80003334:	00200593          	li	a1,2
    80003338:	14459073          	csrw	sip,a1
    8000333c:	01053683          	ld	a3,16(a0)
    80003340:	00853603          	ld	a2,8(a0)
    80003344:	00053583          	ld	a1,0(a0)
    80003348:	34051573          	csrrw	a0,mscratch,a0
    8000334c:	30200073          	mret

0000000080003350 <plicinit>:
    80003350:	ff010113          	addi	sp,sp,-16
    80003354:	00813423          	sd	s0,8(sp)
    80003358:	01010413          	addi	s0,sp,16
    8000335c:	00813403          	ld	s0,8(sp)
    80003360:	0c0007b7          	lui	a5,0xc000
    80003364:	00100713          	li	a4,1
    80003368:	02e7a423          	sw	a4,40(a5) # c000028 <offsetTCBregisters+0xc000018>
    8000336c:	00e7a223          	sw	a4,4(a5)
    80003370:	01010113          	addi	sp,sp,16
    80003374:	00008067          	ret

0000000080003378 <plicinithart>:
    80003378:	ff010113          	addi	sp,sp,-16
    8000337c:	00813023          	sd	s0,0(sp)
    80003380:	00113423          	sd	ra,8(sp)
    80003384:	01010413          	addi	s0,sp,16
    80003388:	00000097          	auipc	ra,0x0
    8000338c:	a48080e7          	jalr	-1464(ra) # 80002dd0 <cpuid>
    80003390:	0085171b          	slliw	a4,a0,0x8
    80003394:	0c0027b7          	lui	a5,0xc002
    80003398:	00e787b3          	add	a5,a5,a4
    8000339c:	40200713          	li	a4,1026
    800033a0:	08e7a023          	sw	a4,128(a5) # c002080 <offsetTCBregisters+0xc002070>
    800033a4:	00813083          	ld	ra,8(sp)
    800033a8:	00013403          	ld	s0,0(sp)
    800033ac:	00d5151b          	slliw	a0,a0,0xd
    800033b0:	0c2017b7          	lui	a5,0xc201
    800033b4:	00a78533          	add	a0,a5,a0
    800033b8:	00052023          	sw	zero,0(a0)
    800033bc:	01010113          	addi	sp,sp,16
    800033c0:	00008067          	ret

00000000800033c4 <plic_claim>:
    800033c4:	ff010113          	addi	sp,sp,-16
    800033c8:	00813023          	sd	s0,0(sp)
    800033cc:	00113423          	sd	ra,8(sp)
    800033d0:	01010413          	addi	s0,sp,16
    800033d4:	00000097          	auipc	ra,0x0
    800033d8:	9fc080e7          	jalr	-1540(ra) # 80002dd0 <cpuid>
    800033dc:	00813083          	ld	ra,8(sp)
    800033e0:	00013403          	ld	s0,0(sp)
    800033e4:	00d5151b          	slliw	a0,a0,0xd
    800033e8:	0c2017b7          	lui	a5,0xc201
    800033ec:	00a78533          	add	a0,a5,a0
    800033f0:	00452503          	lw	a0,4(a0)
    800033f4:	01010113          	addi	sp,sp,16
    800033f8:	00008067          	ret

00000000800033fc <plic_complete>:
    800033fc:	fe010113          	addi	sp,sp,-32
    80003400:	00813823          	sd	s0,16(sp)
    80003404:	00913423          	sd	s1,8(sp)
    80003408:	00113c23          	sd	ra,24(sp)
    8000340c:	02010413          	addi	s0,sp,32
    80003410:	00050493          	mv	s1,a0
    80003414:	00000097          	auipc	ra,0x0
    80003418:	9bc080e7          	jalr	-1604(ra) # 80002dd0 <cpuid>
    8000341c:	01813083          	ld	ra,24(sp)
    80003420:	01013403          	ld	s0,16(sp)
    80003424:	00d5179b          	slliw	a5,a0,0xd
    80003428:	0c201737          	lui	a4,0xc201
    8000342c:	00f707b3          	add	a5,a4,a5
    80003430:	0097a223          	sw	s1,4(a5) # c201004 <offsetTCBregisters+0xc200ff4>
    80003434:	00813483          	ld	s1,8(sp)
    80003438:	02010113          	addi	sp,sp,32
    8000343c:	00008067          	ret

0000000080003440 <consolewrite>:
    80003440:	fb010113          	addi	sp,sp,-80
    80003444:	04813023          	sd	s0,64(sp)
    80003448:	04113423          	sd	ra,72(sp)
    8000344c:	02913c23          	sd	s1,56(sp)
    80003450:	03213823          	sd	s2,48(sp)
    80003454:	03313423          	sd	s3,40(sp)
    80003458:	03413023          	sd	s4,32(sp)
    8000345c:	01513c23          	sd	s5,24(sp)
    80003460:	05010413          	addi	s0,sp,80
    80003464:	06c05c63          	blez	a2,800034dc <consolewrite+0x9c>
    80003468:	00060993          	mv	s3,a2
    8000346c:	00050a13          	mv	s4,a0
    80003470:	00058493          	mv	s1,a1
    80003474:	00000913          	li	s2,0
    80003478:	fff00a93          	li	s5,-1
    8000347c:	01c0006f          	j	80003498 <consolewrite+0x58>
    80003480:	fbf44503          	lbu	a0,-65(s0)
    80003484:	0019091b          	addiw	s2,s2,1
    80003488:	00148493          	addi	s1,s1,1
    8000348c:	00001097          	auipc	ra,0x1
    80003490:	a9c080e7          	jalr	-1380(ra) # 80003f28 <uartputc>
    80003494:	03298063          	beq	s3,s2,800034b4 <consolewrite+0x74>
    80003498:	00048613          	mv	a2,s1
    8000349c:	00100693          	li	a3,1
    800034a0:	000a0593          	mv	a1,s4
    800034a4:	fbf40513          	addi	a0,s0,-65
    800034a8:	00000097          	auipc	ra,0x0
    800034ac:	9e0080e7          	jalr	-1568(ra) # 80002e88 <either_copyin>
    800034b0:	fd5518e3          	bne	a0,s5,80003480 <consolewrite+0x40>
    800034b4:	04813083          	ld	ra,72(sp)
    800034b8:	04013403          	ld	s0,64(sp)
    800034bc:	03813483          	ld	s1,56(sp)
    800034c0:	02813983          	ld	s3,40(sp)
    800034c4:	02013a03          	ld	s4,32(sp)
    800034c8:	01813a83          	ld	s5,24(sp)
    800034cc:	00090513          	mv	a0,s2
    800034d0:	03013903          	ld	s2,48(sp)
    800034d4:	05010113          	addi	sp,sp,80
    800034d8:	00008067          	ret
    800034dc:	00000913          	li	s2,0
    800034e0:	fd5ff06f          	j	800034b4 <consolewrite+0x74>

00000000800034e4 <consoleread>:
    800034e4:	f9010113          	addi	sp,sp,-112
    800034e8:	06813023          	sd	s0,96(sp)
    800034ec:	04913c23          	sd	s1,88(sp)
    800034f0:	05213823          	sd	s2,80(sp)
    800034f4:	05313423          	sd	s3,72(sp)
    800034f8:	05413023          	sd	s4,64(sp)
    800034fc:	03513c23          	sd	s5,56(sp)
    80003500:	03613823          	sd	s6,48(sp)
    80003504:	03713423          	sd	s7,40(sp)
    80003508:	03813023          	sd	s8,32(sp)
    8000350c:	06113423          	sd	ra,104(sp)
    80003510:	01913c23          	sd	s9,24(sp)
    80003514:	07010413          	addi	s0,sp,112
    80003518:	00060b93          	mv	s7,a2
    8000351c:	00050913          	mv	s2,a0
    80003520:	00058c13          	mv	s8,a1
    80003524:	00060b1b          	sext.w	s6,a2
    80003528:	0000c497          	auipc	s1,0xc
    8000352c:	d3048493          	addi	s1,s1,-720 # 8000f258 <cons>
    80003530:	00400993          	li	s3,4
    80003534:	fff00a13          	li	s4,-1
    80003538:	00a00a93          	li	s5,10
    8000353c:	05705e63          	blez	s7,80003598 <consoleread+0xb4>
    80003540:	09c4a703          	lw	a4,156(s1)
    80003544:	0984a783          	lw	a5,152(s1)
    80003548:	0007071b          	sext.w	a4,a4
    8000354c:	08e78463          	beq	a5,a4,800035d4 <consoleread+0xf0>
    80003550:	07f7f713          	andi	a4,a5,127
    80003554:	00e48733          	add	a4,s1,a4
    80003558:	01874703          	lbu	a4,24(a4) # c201018 <offsetTCBregisters+0xc201008>
    8000355c:	0017869b          	addiw	a3,a5,1
    80003560:	08d4ac23          	sw	a3,152(s1)
    80003564:	00070c9b          	sext.w	s9,a4
    80003568:	0b370663          	beq	a4,s3,80003614 <consoleread+0x130>
    8000356c:	00100693          	li	a3,1
    80003570:	f9f40613          	addi	a2,s0,-97
    80003574:	000c0593          	mv	a1,s8
    80003578:	00090513          	mv	a0,s2
    8000357c:	f8e40fa3          	sb	a4,-97(s0)
    80003580:	00000097          	auipc	ra,0x0
    80003584:	8bc080e7          	jalr	-1860(ra) # 80002e3c <either_copyout>
    80003588:	01450863          	beq	a0,s4,80003598 <consoleread+0xb4>
    8000358c:	001c0c13          	addi	s8,s8,1
    80003590:	fffb8b9b          	addiw	s7,s7,-1
    80003594:	fb5c94e3          	bne	s9,s5,8000353c <consoleread+0x58>
    80003598:	000b851b          	sext.w	a0,s7
    8000359c:	06813083          	ld	ra,104(sp)
    800035a0:	06013403          	ld	s0,96(sp)
    800035a4:	05813483          	ld	s1,88(sp)
    800035a8:	05013903          	ld	s2,80(sp)
    800035ac:	04813983          	ld	s3,72(sp)
    800035b0:	04013a03          	ld	s4,64(sp)
    800035b4:	03813a83          	ld	s5,56(sp)
    800035b8:	02813b83          	ld	s7,40(sp)
    800035bc:	02013c03          	ld	s8,32(sp)
    800035c0:	01813c83          	ld	s9,24(sp)
    800035c4:	40ab053b          	subw	a0,s6,a0
    800035c8:	03013b03          	ld	s6,48(sp)
    800035cc:	07010113          	addi	sp,sp,112
    800035d0:	00008067          	ret
    800035d4:	00001097          	auipc	ra,0x1
    800035d8:	1d8080e7          	jalr	472(ra) # 800047ac <push_on>
    800035dc:	0984a703          	lw	a4,152(s1)
    800035e0:	09c4a783          	lw	a5,156(s1)
    800035e4:	0007879b          	sext.w	a5,a5
    800035e8:	fef70ce3          	beq	a4,a5,800035e0 <consoleread+0xfc>
    800035ec:	00001097          	auipc	ra,0x1
    800035f0:	234080e7          	jalr	564(ra) # 80004820 <pop_on>
    800035f4:	0984a783          	lw	a5,152(s1)
    800035f8:	07f7f713          	andi	a4,a5,127
    800035fc:	00e48733          	add	a4,s1,a4
    80003600:	01874703          	lbu	a4,24(a4)
    80003604:	0017869b          	addiw	a3,a5,1
    80003608:	08d4ac23          	sw	a3,152(s1)
    8000360c:	00070c9b          	sext.w	s9,a4
    80003610:	f5371ee3          	bne	a4,s3,8000356c <consoleread+0x88>
    80003614:	000b851b          	sext.w	a0,s7
    80003618:	f96bf2e3          	bgeu	s7,s6,8000359c <consoleread+0xb8>
    8000361c:	08f4ac23          	sw	a5,152(s1)
    80003620:	f7dff06f          	j	8000359c <consoleread+0xb8>

0000000080003624 <consputc>:
    80003624:	10000793          	li	a5,256
    80003628:	00f50663          	beq	a0,a5,80003634 <consputc+0x10>
    8000362c:	00001317          	auipc	t1,0x1
    80003630:	9f430067          	jr	-1548(t1) # 80004020 <uartputc_sync>
    80003634:	ff010113          	addi	sp,sp,-16
    80003638:	00113423          	sd	ra,8(sp)
    8000363c:	00813023          	sd	s0,0(sp)
    80003640:	01010413          	addi	s0,sp,16
    80003644:	00800513          	li	a0,8
    80003648:	00001097          	auipc	ra,0x1
    8000364c:	9d8080e7          	jalr	-1576(ra) # 80004020 <uartputc_sync>
    80003650:	02000513          	li	a0,32
    80003654:	00001097          	auipc	ra,0x1
    80003658:	9cc080e7          	jalr	-1588(ra) # 80004020 <uartputc_sync>
    8000365c:	00013403          	ld	s0,0(sp)
    80003660:	00813083          	ld	ra,8(sp)
    80003664:	00800513          	li	a0,8
    80003668:	01010113          	addi	sp,sp,16
    8000366c:	00001317          	auipc	t1,0x1
    80003670:	9b430067          	jr	-1612(t1) # 80004020 <uartputc_sync>

0000000080003674 <consoleintr>:
    80003674:	fe010113          	addi	sp,sp,-32
    80003678:	00813823          	sd	s0,16(sp)
    8000367c:	00913423          	sd	s1,8(sp)
    80003680:	01213023          	sd	s2,0(sp)
    80003684:	00113c23          	sd	ra,24(sp)
    80003688:	02010413          	addi	s0,sp,32
    8000368c:	0000c917          	auipc	s2,0xc
    80003690:	bcc90913          	addi	s2,s2,-1076 # 8000f258 <cons>
    80003694:	00050493          	mv	s1,a0
    80003698:	00090513          	mv	a0,s2
    8000369c:	00001097          	auipc	ra,0x1
    800036a0:	e40080e7          	jalr	-448(ra) # 800044dc <acquire>
    800036a4:	02048c63          	beqz	s1,800036dc <consoleintr+0x68>
    800036a8:	0a092783          	lw	a5,160(s2)
    800036ac:	09892703          	lw	a4,152(s2)
    800036b0:	07f00693          	li	a3,127
    800036b4:	40e7873b          	subw	a4,a5,a4
    800036b8:	02e6e263          	bltu	a3,a4,800036dc <consoleintr+0x68>
    800036bc:	00d00713          	li	a4,13
    800036c0:	04e48063          	beq	s1,a4,80003700 <consoleintr+0x8c>
    800036c4:	07f7f713          	andi	a4,a5,127
    800036c8:	00e90733          	add	a4,s2,a4
    800036cc:	0017879b          	addiw	a5,a5,1
    800036d0:	0af92023          	sw	a5,160(s2)
    800036d4:	00970c23          	sb	s1,24(a4)
    800036d8:	08f92e23          	sw	a5,156(s2)
    800036dc:	01013403          	ld	s0,16(sp)
    800036e0:	01813083          	ld	ra,24(sp)
    800036e4:	00813483          	ld	s1,8(sp)
    800036e8:	00013903          	ld	s2,0(sp)
    800036ec:	0000c517          	auipc	a0,0xc
    800036f0:	b6c50513          	addi	a0,a0,-1172 # 8000f258 <cons>
    800036f4:	02010113          	addi	sp,sp,32
    800036f8:	00001317          	auipc	t1,0x1
    800036fc:	eb030067          	jr	-336(t1) # 800045a8 <release>
    80003700:	00a00493          	li	s1,10
    80003704:	fc1ff06f          	j	800036c4 <consoleintr+0x50>

0000000080003708 <consoleinit>:
    80003708:	fe010113          	addi	sp,sp,-32
    8000370c:	00113c23          	sd	ra,24(sp)
    80003710:	00813823          	sd	s0,16(sp)
    80003714:	00913423          	sd	s1,8(sp)
    80003718:	02010413          	addi	s0,sp,32
    8000371c:	0000c497          	auipc	s1,0xc
    80003720:	b3c48493          	addi	s1,s1,-1220 # 8000f258 <cons>
    80003724:	00048513          	mv	a0,s1
    80003728:	00002597          	auipc	a1,0x2
    8000372c:	b2058593          	addi	a1,a1,-1248 # 80005248 <CONSOLE_STATUS+0x238>
    80003730:	00001097          	auipc	ra,0x1
    80003734:	d88080e7          	jalr	-632(ra) # 800044b8 <initlock>
    80003738:	00000097          	auipc	ra,0x0
    8000373c:	7ac080e7          	jalr	1964(ra) # 80003ee4 <uartinit>
    80003740:	01813083          	ld	ra,24(sp)
    80003744:	01013403          	ld	s0,16(sp)
    80003748:	00000797          	auipc	a5,0x0
    8000374c:	d9c78793          	addi	a5,a5,-612 # 800034e4 <consoleread>
    80003750:	0af4bc23          	sd	a5,184(s1)
    80003754:	00000797          	auipc	a5,0x0
    80003758:	cec78793          	addi	a5,a5,-788 # 80003440 <consolewrite>
    8000375c:	0cf4b023          	sd	a5,192(s1)
    80003760:	00813483          	ld	s1,8(sp)
    80003764:	02010113          	addi	sp,sp,32
    80003768:	00008067          	ret

000000008000376c <console_read>:
    8000376c:	ff010113          	addi	sp,sp,-16
    80003770:	00813423          	sd	s0,8(sp)
    80003774:	01010413          	addi	s0,sp,16
    80003778:	00813403          	ld	s0,8(sp)
    8000377c:	0000c317          	auipc	t1,0xc
    80003780:	b9433303          	ld	t1,-1132(t1) # 8000f310 <devsw+0x10>
    80003784:	01010113          	addi	sp,sp,16
    80003788:	00030067          	jr	t1

000000008000378c <console_write>:
    8000378c:	ff010113          	addi	sp,sp,-16
    80003790:	00813423          	sd	s0,8(sp)
    80003794:	01010413          	addi	s0,sp,16
    80003798:	00813403          	ld	s0,8(sp)
    8000379c:	0000c317          	auipc	t1,0xc
    800037a0:	b7c33303          	ld	t1,-1156(t1) # 8000f318 <devsw+0x18>
    800037a4:	01010113          	addi	sp,sp,16
    800037a8:	00030067          	jr	t1

00000000800037ac <panic>:
    800037ac:	fe010113          	addi	sp,sp,-32
    800037b0:	00113c23          	sd	ra,24(sp)
    800037b4:	00813823          	sd	s0,16(sp)
    800037b8:	00913423          	sd	s1,8(sp)
    800037bc:	02010413          	addi	s0,sp,32
    800037c0:	00050493          	mv	s1,a0
    800037c4:	00002517          	auipc	a0,0x2
    800037c8:	a8c50513          	addi	a0,a0,-1396 # 80005250 <CONSOLE_STATUS+0x240>
    800037cc:	0000c797          	auipc	a5,0xc
    800037d0:	be07a623          	sw	zero,-1044(a5) # 8000f3b8 <pr+0x18>
    800037d4:	00000097          	auipc	ra,0x0
    800037d8:	034080e7          	jalr	52(ra) # 80003808 <__printf>
    800037dc:	00048513          	mv	a0,s1
    800037e0:	00000097          	auipc	ra,0x0
    800037e4:	028080e7          	jalr	40(ra) # 80003808 <__printf>
    800037e8:	00002517          	auipc	a0,0x2
    800037ec:	a4850513          	addi	a0,a0,-1464 # 80005230 <CONSOLE_STATUS+0x220>
    800037f0:	00000097          	auipc	ra,0x0
    800037f4:	018080e7          	jalr	24(ra) # 80003808 <__printf>
    800037f8:	00100793          	li	a5,1
    800037fc:	00003717          	auipc	a4,0x3
    80003800:	90f72623          	sw	a5,-1780(a4) # 80006108 <panicked>
    80003804:	0000006f          	j	80003804 <panic+0x58>

0000000080003808 <__printf>:
    80003808:	f3010113          	addi	sp,sp,-208
    8000380c:	08813023          	sd	s0,128(sp)
    80003810:	07313423          	sd	s3,104(sp)
    80003814:	09010413          	addi	s0,sp,144
    80003818:	05813023          	sd	s8,64(sp)
    8000381c:	08113423          	sd	ra,136(sp)
    80003820:	06913c23          	sd	s1,120(sp)
    80003824:	07213823          	sd	s2,112(sp)
    80003828:	07413023          	sd	s4,96(sp)
    8000382c:	05513c23          	sd	s5,88(sp)
    80003830:	05613823          	sd	s6,80(sp)
    80003834:	05713423          	sd	s7,72(sp)
    80003838:	03913c23          	sd	s9,56(sp)
    8000383c:	03a13823          	sd	s10,48(sp)
    80003840:	03b13423          	sd	s11,40(sp)
    80003844:	0000c317          	auipc	t1,0xc
    80003848:	b5c30313          	addi	t1,t1,-1188 # 8000f3a0 <pr>
    8000384c:	01832c03          	lw	s8,24(t1)
    80003850:	00b43423          	sd	a1,8(s0)
    80003854:	00c43823          	sd	a2,16(s0)
    80003858:	00d43c23          	sd	a3,24(s0)
    8000385c:	02e43023          	sd	a4,32(s0)
    80003860:	02f43423          	sd	a5,40(s0)
    80003864:	03043823          	sd	a6,48(s0)
    80003868:	03143c23          	sd	a7,56(s0)
    8000386c:	00050993          	mv	s3,a0
    80003870:	4a0c1663          	bnez	s8,80003d1c <__printf+0x514>
    80003874:	60098c63          	beqz	s3,80003e8c <__printf+0x684>
    80003878:	0009c503          	lbu	a0,0(s3)
    8000387c:	00840793          	addi	a5,s0,8
    80003880:	f6f43c23          	sd	a5,-136(s0)
    80003884:	00000493          	li	s1,0
    80003888:	22050063          	beqz	a0,80003aa8 <__printf+0x2a0>
    8000388c:	00002a37          	lui	s4,0x2
    80003890:	00018ab7          	lui	s5,0x18
    80003894:	000f4b37          	lui	s6,0xf4
    80003898:	00989bb7          	lui	s7,0x989
    8000389c:	70fa0a13          	addi	s4,s4,1807 # 270f <offsetTCBregisters+0x26ff>
    800038a0:	69fa8a93          	addi	s5,s5,1695 # 1869f <offsetTCBregisters+0x1868f>
    800038a4:	23fb0b13          	addi	s6,s6,575 # f423f <offsetTCBregisters+0xf422f>
    800038a8:	67fb8b93          	addi	s7,s7,1663 # 98967f <offsetTCBregisters+0x98966f>
    800038ac:	00148c9b          	addiw	s9,s1,1
    800038b0:	02500793          	li	a5,37
    800038b4:	01998933          	add	s2,s3,s9
    800038b8:	38f51263          	bne	a0,a5,80003c3c <__printf+0x434>
    800038bc:	00094783          	lbu	a5,0(s2)
    800038c0:	00078c9b          	sext.w	s9,a5
    800038c4:	1e078263          	beqz	a5,80003aa8 <__printf+0x2a0>
    800038c8:	0024849b          	addiw	s1,s1,2
    800038cc:	07000713          	li	a4,112
    800038d0:	00998933          	add	s2,s3,s1
    800038d4:	38e78a63          	beq	a5,a4,80003c68 <__printf+0x460>
    800038d8:	20f76863          	bltu	a4,a5,80003ae8 <__printf+0x2e0>
    800038dc:	42a78863          	beq	a5,a0,80003d0c <__printf+0x504>
    800038e0:	06400713          	li	a4,100
    800038e4:	40e79663          	bne	a5,a4,80003cf0 <__printf+0x4e8>
    800038e8:	f7843783          	ld	a5,-136(s0)
    800038ec:	0007a603          	lw	a2,0(a5)
    800038f0:	00878793          	addi	a5,a5,8
    800038f4:	f6f43c23          	sd	a5,-136(s0)
    800038f8:	42064a63          	bltz	a2,80003d2c <__printf+0x524>
    800038fc:	00a00713          	li	a4,10
    80003900:	02e677bb          	remuw	a5,a2,a4
    80003904:	00002d97          	auipc	s11,0x2
    80003908:	974d8d93          	addi	s11,s11,-1676 # 80005278 <digits>
    8000390c:	00900593          	li	a1,9
    80003910:	0006051b          	sext.w	a0,a2
    80003914:	00000c93          	li	s9,0
    80003918:	02079793          	slli	a5,a5,0x20
    8000391c:	0207d793          	srli	a5,a5,0x20
    80003920:	00fd87b3          	add	a5,s11,a5
    80003924:	0007c783          	lbu	a5,0(a5)
    80003928:	02e656bb          	divuw	a3,a2,a4
    8000392c:	f8f40023          	sb	a5,-128(s0)
    80003930:	14c5d863          	bge	a1,a2,80003a80 <__printf+0x278>
    80003934:	06300593          	li	a1,99
    80003938:	00100c93          	li	s9,1
    8000393c:	02e6f7bb          	remuw	a5,a3,a4
    80003940:	02079793          	slli	a5,a5,0x20
    80003944:	0207d793          	srli	a5,a5,0x20
    80003948:	00fd87b3          	add	a5,s11,a5
    8000394c:	0007c783          	lbu	a5,0(a5)
    80003950:	02e6d73b          	divuw	a4,a3,a4
    80003954:	f8f400a3          	sb	a5,-127(s0)
    80003958:	12a5f463          	bgeu	a1,a0,80003a80 <__printf+0x278>
    8000395c:	00a00693          	li	a3,10
    80003960:	00900593          	li	a1,9
    80003964:	02d777bb          	remuw	a5,a4,a3
    80003968:	02079793          	slli	a5,a5,0x20
    8000396c:	0207d793          	srli	a5,a5,0x20
    80003970:	00fd87b3          	add	a5,s11,a5
    80003974:	0007c503          	lbu	a0,0(a5)
    80003978:	02d757bb          	divuw	a5,a4,a3
    8000397c:	f8a40123          	sb	a0,-126(s0)
    80003980:	48e5f263          	bgeu	a1,a4,80003e04 <__printf+0x5fc>
    80003984:	06300513          	li	a0,99
    80003988:	02d7f5bb          	remuw	a1,a5,a3
    8000398c:	02059593          	slli	a1,a1,0x20
    80003990:	0205d593          	srli	a1,a1,0x20
    80003994:	00bd85b3          	add	a1,s11,a1
    80003998:	0005c583          	lbu	a1,0(a1)
    8000399c:	02d7d7bb          	divuw	a5,a5,a3
    800039a0:	f8b401a3          	sb	a1,-125(s0)
    800039a4:	48e57263          	bgeu	a0,a4,80003e28 <__printf+0x620>
    800039a8:	3e700513          	li	a0,999
    800039ac:	02d7f5bb          	remuw	a1,a5,a3
    800039b0:	02059593          	slli	a1,a1,0x20
    800039b4:	0205d593          	srli	a1,a1,0x20
    800039b8:	00bd85b3          	add	a1,s11,a1
    800039bc:	0005c583          	lbu	a1,0(a1)
    800039c0:	02d7d7bb          	divuw	a5,a5,a3
    800039c4:	f8b40223          	sb	a1,-124(s0)
    800039c8:	46e57663          	bgeu	a0,a4,80003e34 <__printf+0x62c>
    800039cc:	02d7f5bb          	remuw	a1,a5,a3
    800039d0:	02059593          	slli	a1,a1,0x20
    800039d4:	0205d593          	srli	a1,a1,0x20
    800039d8:	00bd85b3          	add	a1,s11,a1
    800039dc:	0005c583          	lbu	a1,0(a1)
    800039e0:	02d7d7bb          	divuw	a5,a5,a3
    800039e4:	f8b402a3          	sb	a1,-123(s0)
    800039e8:	46ea7863          	bgeu	s4,a4,80003e58 <__printf+0x650>
    800039ec:	02d7f5bb          	remuw	a1,a5,a3
    800039f0:	02059593          	slli	a1,a1,0x20
    800039f4:	0205d593          	srli	a1,a1,0x20
    800039f8:	00bd85b3          	add	a1,s11,a1
    800039fc:	0005c583          	lbu	a1,0(a1)
    80003a00:	02d7d7bb          	divuw	a5,a5,a3
    80003a04:	f8b40323          	sb	a1,-122(s0)
    80003a08:	3eeaf863          	bgeu	s5,a4,80003df8 <__printf+0x5f0>
    80003a0c:	02d7f5bb          	remuw	a1,a5,a3
    80003a10:	02059593          	slli	a1,a1,0x20
    80003a14:	0205d593          	srli	a1,a1,0x20
    80003a18:	00bd85b3          	add	a1,s11,a1
    80003a1c:	0005c583          	lbu	a1,0(a1)
    80003a20:	02d7d7bb          	divuw	a5,a5,a3
    80003a24:	f8b403a3          	sb	a1,-121(s0)
    80003a28:	42eb7e63          	bgeu	s6,a4,80003e64 <__printf+0x65c>
    80003a2c:	02d7f5bb          	remuw	a1,a5,a3
    80003a30:	02059593          	slli	a1,a1,0x20
    80003a34:	0205d593          	srli	a1,a1,0x20
    80003a38:	00bd85b3          	add	a1,s11,a1
    80003a3c:	0005c583          	lbu	a1,0(a1)
    80003a40:	02d7d7bb          	divuw	a5,a5,a3
    80003a44:	f8b40423          	sb	a1,-120(s0)
    80003a48:	42ebfc63          	bgeu	s7,a4,80003e80 <__printf+0x678>
    80003a4c:	02079793          	slli	a5,a5,0x20
    80003a50:	0207d793          	srli	a5,a5,0x20
    80003a54:	00fd8db3          	add	s11,s11,a5
    80003a58:	000dc703          	lbu	a4,0(s11)
    80003a5c:	00a00793          	li	a5,10
    80003a60:	00900c93          	li	s9,9
    80003a64:	f8e404a3          	sb	a4,-119(s0)
    80003a68:	00065c63          	bgez	a2,80003a80 <__printf+0x278>
    80003a6c:	f9040713          	addi	a4,s0,-112
    80003a70:	00f70733          	add	a4,a4,a5
    80003a74:	02d00693          	li	a3,45
    80003a78:	fed70823          	sb	a3,-16(a4)
    80003a7c:	00078c93          	mv	s9,a5
    80003a80:	f8040793          	addi	a5,s0,-128
    80003a84:	01978cb3          	add	s9,a5,s9
    80003a88:	f7f40d13          	addi	s10,s0,-129
    80003a8c:	000cc503          	lbu	a0,0(s9)
    80003a90:	fffc8c93          	addi	s9,s9,-1
    80003a94:	00000097          	auipc	ra,0x0
    80003a98:	b90080e7          	jalr	-1136(ra) # 80003624 <consputc>
    80003a9c:	ffac98e3          	bne	s9,s10,80003a8c <__printf+0x284>
    80003aa0:	00094503          	lbu	a0,0(s2)
    80003aa4:	e00514e3          	bnez	a0,800038ac <__printf+0xa4>
    80003aa8:	1a0c1663          	bnez	s8,80003c54 <__printf+0x44c>
    80003aac:	08813083          	ld	ra,136(sp)
    80003ab0:	08013403          	ld	s0,128(sp)
    80003ab4:	07813483          	ld	s1,120(sp)
    80003ab8:	07013903          	ld	s2,112(sp)
    80003abc:	06813983          	ld	s3,104(sp)
    80003ac0:	06013a03          	ld	s4,96(sp)
    80003ac4:	05813a83          	ld	s5,88(sp)
    80003ac8:	05013b03          	ld	s6,80(sp)
    80003acc:	04813b83          	ld	s7,72(sp)
    80003ad0:	04013c03          	ld	s8,64(sp)
    80003ad4:	03813c83          	ld	s9,56(sp)
    80003ad8:	03013d03          	ld	s10,48(sp)
    80003adc:	02813d83          	ld	s11,40(sp)
    80003ae0:	0d010113          	addi	sp,sp,208
    80003ae4:	00008067          	ret
    80003ae8:	07300713          	li	a4,115
    80003aec:	1ce78a63          	beq	a5,a4,80003cc0 <__printf+0x4b8>
    80003af0:	07800713          	li	a4,120
    80003af4:	1ee79e63          	bne	a5,a4,80003cf0 <__printf+0x4e8>
    80003af8:	f7843783          	ld	a5,-136(s0)
    80003afc:	0007a703          	lw	a4,0(a5)
    80003b00:	00878793          	addi	a5,a5,8
    80003b04:	f6f43c23          	sd	a5,-136(s0)
    80003b08:	28074263          	bltz	a4,80003d8c <__printf+0x584>
    80003b0c:	00001d97          	auipc	s11,0x1
    80003b10:	76cd8d93          	addi	s11,s11,1900 # 80005278 <digits>
    80003b14:	00f77793          	andi	a5,a4,15
    80003b18:	00fd87b3          	add	a5,s11,a5
    80003b1c:	0007c683          	lbu	a3,0(a5)
    80003b20:	00f00613          	li	a2,15
    80003b24:	0007079b          	sext.w	a5,a4
    80003b28:	f8d40023          	sb	a3,-128(s0)
    80003b2c:	0047559b          	srliw	a1,a4,0x4
    80003b30:	0047569b          	srliw	a3,a4,0x4
    80003b34:	00000c93          	li	s9,0
    80003b38:	0ee65063          	bge	a2,a4,80003c18 <__printf+0x410>
    80003b3c:	00f6f693          	andi	a3,a3,15
    80003b40:	00dd86b3          	add	a3,s11,a3
    80003b44:	0006c683          	lbu	a3,0(a3) # 2004000 <offsetTCBregisters+0x2003ff0>
    80003b48:	0087d79b          	srliw	a5,a5,0x8
    80003b4c:	00100c93          	li	s9,1
    80003b50:	f8d400a3          	sb	a3,-127(s0)
    80003b54:	0cb67263          	bgeu	a2,a1,80003c18 <__printf+0x410>
    80003b58:	00f7f693          	andi	a3,a5,15
    80003b5c:	00dd86b3          	add	a3,s11,a3
    80003b60:	0006c583          	lbu	a1,0(a3)
    80003b64:	00f00613          	li	a2,15
    80003b68:	0047d69b          	srliw	a3,a5,0x4
    80003b6c:	f8b40123          	sb	a1,-126(s0)
    80003b70:	0047d593          	srli	a1,a5,0x4
    80003b74:	28f67e63          	bgeu	a2,a5,80003e10 <__printf+0x608>
    80003b78:	00f6f693          	andi	a3,a3,15
    80003b7c:	00dd86b3          	add	a3,s11,a3
    80003b80:	0006c503          	lbu	a0,0(a3)
    80003b84:	0087d813          	srli	a6,a5,0x8
    80003b88:	0087d69b          	srliw	a3,a5,0x8
    80003b8c:	f8a401a3          	sb	a0,-125(s0)
    80003b90:	28b67663          	bgeu	a2,a1,80003e1c <__printf+0x614>
    80003b94:	00f6f693          	andi	a3,a3,15
    80003b98:	00dd86b3          	add	a3,s11,a3
    80003b9c:	0006c583          	lbu	a1,0(a3)
    80003ba0:	00c7d513          	srli	a0,a5,0xc
    80003ba4:	00c7d69b          	srliw	a3,a5,0xc
    80003ba8:	f8b40223          	sb	a1,-124(s0)
    80003bac:	29067a63          	bgeu	a2,a6,80003e40 <__printf+0x638>
    80003bb0:	00f6f693          	andi	a3,a3,15
    80003bb4:	00dd86b3          	add	a3,s11,a3
    80003bb8:	0006c583          	lbu	a1,0(a3)
    80003bbc:	0107d813          	srli	a6,a5,0x10
    80003bc0:	0107d69b          	srliw	a3,a5,0x10
    80003bc4:	f8b402a3          	sb	a1,-123(s0)
    80003bc8:	28a67263          	bgeu	a2,a0,80003e4c <__printf+0x644>
    80003bcc:	00f6f693          	andi	a3,a3,15
    80003bd0:	00dd86b3          	add	a3,s11,a3
    80003bd4:	0006c683          	lbu	a3,0(a3)
    80003bd8:	0147d79b          	srliw	a5,a5,0x14
    80003bdc:	f8d40323          	sb	a3,-122(s0)
    80003be0:	21067663          	bgeu	a2,a6,80003dec <__printf+0x5e4>
    80003be4:	02079793          	slli	a5,a5,0x20
    80003be8:	0207d793          	srli	a5,a5,0x20
    80003bec:	00fd8db3          	add	s11,s11,a5
    80003bf0:	000dc683          	lbu	a3,0(s11)
    80003bf4:	00800793          	li	a5,8
    80003bf8:	00700c93          	li	s9,7
    80003bfc:	f8d403a3          	sb	a3,-121(s0)
    80003c00:	00075c63          	bgez	a4,80003c18 <__printf+0x410>
    80003c04:	f9040713          	addi	a4,s0,-112
    80003c08:	00f70733          	add	a4,a4,a5
    80003c0c:	02d00693          	li	a3,45
    80003c10:	fed70823          	sb	a3,-16(a4)
    80003c14:	00078c93          	mv	s9,a5
    80003c18:	f8040793          	addi	a5,s0,-128
    80003c1c:	01978cb3          	add	s9,a5,s9
    80003c20:	f7f40d13          	addi	s10,s0,-129
    80003c24:	000cc503          	lbu	a0,0(s9)
    80003c28:	fffc8c93          	addi	s9,s9,-1
    80003c2c:	00000097          	auipc	ra,0x0
    80003c30:	9f8080e7          	jalr	-1544(ra) # 80003624 <consputc>
    80003c34:	ff9d18e3          	bne	s10,s9,80003c24 <__printf+0x41c>
    80003c38:	0100006f          	j	80003c48 <__printf+0x440>
    80003c3c:	00000097          	auipc	ra,0x0
    80003c40:	9e8080e7          	jalr	-1560(ra) # 80003624 <consputc>
    80003c44:	000c8493          	mv	s1,s9
    80003c48:	00094503          	lbu	a0,0(s2)
    80003c4c:	c60510e3          	bnez	a0,800038ac <__printf+0xa4>
    80003c50:	e40c0ee3          	beqz	s8,80003aac <__printf+0x2a4>
    80003c54:	0000b517          	auipc	a0,0xb
    80003c58:	74c50513          	addi	a0,a0,1868 # 8000f3a0 <pr>
    80003c5c:	00001097          	auipc	ra,0x1
    80003c60:	94c080e7          	jalr	-1716(ra) # 800045a8 <release>
    80003c64:	e49ff06f          	j	80003aac <__printf+0x2a4>
    80003c68:	f7843783          	ld	a5,-136(s0)
    80003c6c:	03000513          	li	a0,48
    80003c70:	01000d13          	li	s10,16
    80003c74:	00878713          	addi	a4,a5,8
    80003c78:	0007bc83          	ld	s9,0(a5)
    80003c7c:	f6e43c23          	sd	a4,-136(s0)
    80003c80:	00000097          	auipc	ra,0x0
    80003c84:	9a4080e7          	jalr	-1628(ra) # 80003624 <consputc>
    80003c88:	07800513          	li	a0,120
    80003c8c:	00000097          	auipc	ra,0x0
    80003c90:	998080e7          	jalr	-1640(ra) # 80003624 <consputc>
    80003c94:	00001d97          	auipc	s11,0x1
    80003c98:	5e4d8d93          	addi	s11,s11,1508 # 80005278 <digits>
    80003c9c:	03ccd793          	srli	a5,s9,0x3c
    80003ca0:	00fd87b3          	add	a5,s11,a5
    80003ca4:	0007c503          	lbu	a0,0(a5)
    80003ca8:	fffd0d1b          	addiw	s10,s10,-1
    80003cac:	004c9c93          	slli	s9,s9,0x4
    80003cb0:	00000097          	auipc	ra,0x0
    80003cb4:	974080e7          	jalr	-1676(ra) # 80003624 <consputc>
    80003cb8:	fe0d12e3          	bnez	s10,80003c9c <__printf+0x494>
    80003cbc:	f8dff06f          	j	80003c48 <__printf+0x440>
    80003cc0:	f7843783          	ld	a5,-136(s0)
    80003cc4:	0007bc83          	ld	s9,0(a5)
    80003cc8:	00878793          	addi	a5,a5,8
    80003ccc:	f6f43c23          	sd	a5,-136(s0)
    80003cd0:	000c9a63          	bnez	s9,80003ce4 <__printf+0x4dc>
    80003cd4:	1080006f          	j	80003ddc <__printf+0x5d4>
    80003cd8:	001c8c93          	addi	s9,s9,1
    80003cdc:	00000097          	auipc	ra,0x0
    80003ce0:	948080e7          	jalr	-1720(ra) # 80003624 <consputc>
    80003ce4:	000cc503          	lbu	a0,0(s9)
    80003ce8:	fe0518e3          	bnez	a0,80003cd8 <__printf+0x4d0>
    80003cec:	f5dff06f          	j	80003c48 <__printf+0x440>
    80003cf0:	02500513          	li	a0,37
    80003cf4:	00000097          	auipc	ra,0x0
    80003cf8:	930080e7          	jalr	-1744(ra) # 80003624 <consputc>
    80003cfc:	000c8513          	mv	a0,s9
    80003d00:	00000097          	auipc	ra,0x0
    80003d04:	924080e7          	jalr	-1756(ra) # 80003624 <consputc>
    80003d08:	f41ff06f          	j	80003c48 <__printf+0x440>
    80003d0c:	02500513          	li	a0,37
    80003d10:	00000097          	auipc	ra,0x0
    80003d14:	914080e7          	jalr	-1772(ra) # 80003624 <consputc>
    80003d18:	f31ff06f          	j	80003c48 <__printf+0x440>
    80003d1c:	00030513          	mv	a0,t1
    80003d20:	00000097          	auipc	ra,0x0
    80003d24:	7bc080e7          	jalr	1980(ra) # 800044dc <acquire>
    80003d28:	b4dff06f          	j	80003874 <__printf+0x6c>
    80003d2c:	40c0053b          	negw	a0,a2
    80003d30:	00a00713          	li	a4,10
    80003d34:	02e576bb          	remuw	a3,a0,a4
    80003d38:	00001d97          	auipc	s11,0x1
    80003d3c:	540d8d93          	addi	s11,s11,1344 # 80005278 <digits>
    80003d40:	ff700593          	li	a1,-9
    80003d44:	02069693          	slli	a3,a3,0x20
    80003d48:	0206d693          	srli	a3,a3,0x20
    80003d4c:	00dd86b3          	add	a3,s11,a3
    80003d50:	0006c683          	lbu	a3,0(a3)
    80003d54:	02e557bb          	divuw	a5,a0,a4
    80003d58:	f8d40023          	sb	a3,-128(s0)
    80003d5c:	10b65e63          	bge	a2,a1,80003e78 <__printf+0x670>
    80003d60:	06300593          	li	a1,99
    80003d64:	02e7f6bb          	remuw	a3,a5,a4
    80003d68:	02069693          	slli	a3,a3,0x20
    80003d6c:	0206d693          	srli	a3,a3,0x20
    80003d70:	00dd86b3          	add	a3,s11,a3
    80003d74:	0006c683          	lbu	a3,0(a3)
    80003d78:	02e7d73b          	divuw	a4,a5,a4
    80003d7c:	00200793          	li	a5,2
    80003d80:	f8d400a3          	sb	a3,-127(s0)
    80003d84:	bca5ece3          	bltu	a1,a0,8000395c <__printf+0x154>
    80003d88:	ce5ff06f          	j	80003a6c <__printf+0x264>
    80003d8c:	40e007bb          	negw	a5,a4
    80003d90:	00001d97          	auipc	s11,0x1
    80003d94:	4e8d8d93          	addi	s11,s11,1256 # 80005278 <digits>
    80003d98:	00f7f693          	andi	a3,a5,15
    80003d9c:	00dd86b3          	add	a3,s11,a3
    80003da0:	0006c583          	lbu	a1,0(a3)
    80003da4:	ff100613          	li	a2,-15
    80003da8:	0047d69b          	srliw	a3,a5,0x4
    80003dac:	f8b40023          	sb	a1,-128(s0)
    80003db0:	0047d59b          	srliw	a1,a5,0x4
    80003db4:	0ac75e63          	bge	a4,a2,80003e70 <__printf+0x668>
    80003db8:	00f6f693          	andi	a3,a3,15
    80003dbc:	00dd86b3          	add	a3,s11,a3
    80003dc0:	0006c603          	lbu	a2,0(a3)
    80003dc4:	00f00693          	li	a3,15
    80003dc8:	0087d79b          	srliw	a5,a5,0x8
    80003dcc:	f8c400a3          	sb	a2,-127(s0)
    80003dd0:	d8b6e4e3          	bltu	a3,a1,80003b58 <__printf+0x350>
    80003dd4:	00200793          	li	a5,2
    80003dd8:	e2dff06f          	j	80003c04 <__printf+0x3fc>
    80003ddc:	00001c97          	auipc	s9,0x1
    80003de0:	47cc8c93          	addi	s9,s9,1148 # 80005258 <CONSOLE_STATUS+0x248>
    80003de4:	02800513          	li	a0,40
    80003de8:	ef1ff06f          	j	80003cd8 <__printf+0x4d0>
    80003dec:	00700793          	li	a5,7
    80003df0:	00600c93          	li	s9,6
    80003df4:	e0dff06f          	j	80003c00 <__printf+0x3f8>
    80003df8:	00700793          	li	a5,7
    80003dfc:	00600c93          	li	s9,6
    80003e00:	c69ff06f          	j	80003a68 <__printf+0x260>
    80003e04:	00300793          	li	a5,3
    80003e08:	00200c93          	li	s9,2
    80003e0c:	c5dff06f          	j	80003a68 <__printf+0x260>
    80003e10:	00300793          	li	a5,3
    80003e14:	00200c93          	li	s9,2
    80003e18:	de9ff06f          	j	80003c00 <__printf+0x3f8>
    80003e1c:	00400793          	li	a5,4
    80003e20:	00300c93          	li	s9,3
    80003e24:	dddff06f          	j	80003c00 <__printf+0x3f8>
    80003e28:	00400793          	li	a5,4
    80003e2c:	00300c93          	li	s9,3
    80003e30:	c39ff06f          	j	80003a68 <__printf+0x260>
    80003e34:	00500793          	li	a5,5
    80003e38:	00400c93          	li	s9,4
    80003e3c:	c2dff06f          	j	80003a68 <__printf+0x260>
    80003e40:	00500793          	li	a5,5
    80003e44:	00400c93          	li	s9,4
    80003e48:	db9ff06f          	j	80003c00 <__printf+0x3f8>
    80003e4c:	00600793          	li	a5,6
    80003e50:	00500c93          	li	s9,5
    80003e54:	dadff06f          	j	80003c00 <__printf+0x3f8>
    80003e58:	00600793          	li	a5,6
    80003e5c:	00500c93          	li	s9,5
    80003e60:	c09ff06f          	j	80003a68 <__printf+0x260>
    80003e64:	00800793          	li	a5,8
    80003e68:	00700c93          	li	s9,7
    80003e6c:	bfdff06f          	j	80003a68 <__printf+0x260>
    80003e70:	00100793          	li	a5,1
    80003e74:	d91ff06f          	j	80003c04 <__printf+0x3fc>
    80003e78:	00100793          	li	a5,1
    80003e7c:	bf1ff06f          	j	80003a6c <__printf+0x264>
    80003e80:	00900793          	li	a5,9
    80003e84:	00800c93          	li	s9,8
    80003e88:	be1ff06f          	j	80003a68 <__printf+0x260>
    80003e8c:	00001517          	auipc	a0,0x1
    80003e90:	3d450513          	addi	a0,a0,980 # 80005260 <CONSOLE_STATUS+0x250>
    80003e94:	00000097          	auipc	ra,0x0
    80003e98:	918080e7          	jalr	-1768(ra) # 800037ac <panic>

0000000080003e9c <printfinit>:
    80003e9c:	fe010113          	addi	sp,sp,-32
    80003ea0:	00813823          	sd	s0,16(sp)
    80003ea4:	00913423          	sd	s1,8(sp)
    80003ea8:	00113c23          	sd	ra,24(sp)
    80003eac:	02010413          	addi	s0,sp,32
    80003eb0:	0000b497          	auipc	s1,0xb
    80003eb4:	4f048493          	addi	s1,s1,1264 # 8000f3a0 <pr>
    80003eb8:	00048513          	mv	a0,s1
    80003ebc:	00001597          	auipc	a1,0x1
    80003ec0:	3b458593          	addi	a1,a1,948 # 80005270 <CONSOLE_STATUS+0x260>
    80003ec4:	00000097          	auipc	ra,0x0
    80003ec8:	5f4080e7          	jalr	1524(ra) # 800044b8 <initlock>
    80003ecc:	01813083          	ld	ra,24(sp)
    80003ed0:	01013403          	ld	s0,16(sp)
    80003ed4:	0004ac23          	sw	zero,24(s1)
    80003ed8:	00813483          	ld	s1,8(sp)
    80003edc:	02010113          	addi	sp,sp,32
    80003ee0:	00008067          	ret

0000000080003ee4 <uartinit>:
    80003ee4:	ff010113          	addi	sp,sp,-16
    80003ee8:	00813423          	sd	s0,8(sp)
    80003eec:	01010413          	addi	s0,sp,16
    80003ef0:	100007b7          	lui	a5,0x10000
    80003ef4:	000780a3          	sb	zero,1(a5) # 10000001 <offsetTCBregisters+0xffffff1>
    80003ef8:	f8000713          	li	a4,-128
    80003efc:	00e781a3          	sb	a4,3(a5)
    80003f00:	00300713          	li	a4,3
    80003f04:	00e78023          	sb	a4,0(a5)
    80003f08:	000780a3          	sb	zero,1(a5)
    80003f0c:	00e781a3          	sb	a4,3(a5)
    80003f10:	00700693          	li	a3,7
    80003f14:	00d78123          	sb	a3,2(a5)
    80003f18:	00e780a3          	sb	a4,1(a5)
    80003f1c:	00813403          	ld	s0,8(sp)
    80003f20:	01010113          	addi	sp,sp,16
    80003f24:	00008067          	ret

0000000080003f28 <uartputc>:
    80003f28:	00002797          	auipc	a5,0x2
    80003f2c:	1e07a783          	lw	a5,480(a5) # 80006108 <panicked>
    80003f30:	00078463          	beqz	a5,80003f38 <uartputc+0x10>
    80003f34:	0000006f          	j	80003f34 <uartputc+0xc>
    80003f38:	fd010113          	addi	sp,sp,-48
    80003f3c:	02813023          	sd	s0,32(sp)
    80003f40:	00913c23          	sd	s1,24(sp)
    80003f44:	01213823          	sd	s2,16(sp)
    80003f48:	01313423          	sd	s3,8(sp)
    80003f4c:	02113423          	sd	ra,40(sp)
    80003f50:	03010413          	addi	s0,sp,48
    80003f54:	00002917          	auipc	s2,0x2
    80003f58:	1bc90913          	addi	s2,s2,444 # 80006110 <uart_tx_r>
    80003f5c:	00093783          	ld	a5,0(s2)
    80003f60:	00002497          	auipc	s1,0x2
    80003f64:	1b848493          	addi	s1,s1,440 # 80006118 <uart_tx_w>
    80003f68:	0004b703          	ld	a4,0(s1)
    80003f6c:	02078693          	addi	a3,a5,32
    80003f70:	00050993          	mv	s3,a0
    80003f74:	02e69c63          	bne	a3,a4,80003fac <uartputc+0x84>
    80003f78:	00001097          	auipc	ra,0x1
    80003f7c:	834080e7          	jalr	-1996(ra) # 800047ac <push_on>
    80003f80:	00093783          	ld	a5,0(s2)
    80003f84:	0004b703          	ld	a4,0(s1)
    80003f88:	02078793          	addi	a5,a5,32
    80003f8c:	00e79463          	bne	a5,a4,80003f94 <uartputc+0x6c>
    80003f90:	0000006f          	j	80003f90 <uartputc+0x68>
    80003f94:	00001097          	auipc	ra,0x1
    80003f98:	88c080e7          	jalr	-1908(ra) # 80004820 <pop_on>
    80003f9c:	00093783          	ld	a5,0(s2)
    80003fa0:	0004b703          	ld	a4,0(s1)
    80003fa4:	02078693          	addi	a3,a5,32
    80003fa8:	fce688e3          	beq	a3,a4,80003f78 <uartputc+0x50>
    80003fac:	01f77693          	andi	a3,a4,31
    80003fb0:	0000b597          	auipc	a1,0xb
    80003fb4:	41058593          	addi	a1,a1,1040 # 8000f3c0 <uart_tx_buf>
    80003fb8:	00d586b3          	add	a3,a1,a3
    80003fbc:	00170713          	addi	a4,a4,1
    80003fc0:	01368023          	sb	s3,0(a3)
    80003fc4:	00e4b023          	sd	a4,0(s1)
    80003fc8:	10000637          	lui	a2,0x10000
    80003fcc:	02f71063          	bne	a4,a5,80003fec <uartputc+0xc4>
    80003fd0:	0340006f          	j	80004004 <uartputc+0xdc>
    80003fd4:	00074703          	lbu	a4,0(a4)
    80003fd8:	00f93023          	sd	a5,0(s2)
    80003fdc:	00e60023          	sb	a4,0(a2) # 10000000 <offsetTCBregisters+0xffffff0>
    80003fe0:	00093783          	ld	a5,0(s2)
    80003fe4:	0004b703          	ld	a4,0(s1)
    80003fe8:	00f70e63          	beq	a4,a5,80004004 <uartputc+0xdc>
    80003fec:	00564683          	lbu	a3,5(a2)
    80003ff0:	01f7f713          	andi	a4,a5,31
    80003ff4:	00e58733          	add	a4,a1,a4
    80003ff8:	0206f693          	andi	a3,a3,32
    80003ffc:	00178793          	addi	a5,a5,1
    80004000:	fc069ae3          	bnez	a3,80003fd4 <uartputc+0xac>
    80004004:	02813083          	ld	ra,40(sp)
    80004008:	02013403          	ld	s0,32(sp)
    8000400c:	01813483          	ld	s1,24(sp)
    80004010:	01013903          	ld	s2,16(sp)
    80004014:	00813983          	ld	s3,8(sp)
    80004018:	03010113          	addi	sp,sp,48
    8000401c:	00008067          	ret

0000000080004020 <uartputc_sync>:
    80004020:	ff010113          	addi	sp,sp,-16
    80004024:	00813423          	sd	s0,8(sp)
    80004028:	01010413          	addi	s0,sp,16
    8000402c:	00002717          	auipc	a4,0x2
    80004030:	0dc72703          	lw	a4,220(a4) # 80006108 <panicked>
    80004034:	02071663          	bnez	a4,80004060 <uartputc_sync+0x40>
    80004038:	00050793          	mv	a5,a0
    8000403c:	100006b7          	lui	a3,0x10000
    80004040:	0056c703          	lbu	a4,5(a3) # 10000005 <offsetTCBregisters+0xffffff5>
    80004044:	02077713          	andi	a4,a4,32
    80004048:	fe070ce3          	beqz	a4,80004040 <uartputc_sync+0x20>
    8000404c:	0ff7f793          	andi	a5,a5,255
    80004050:	00f68023          	sb	a5,0(a3)
    80004054:	00813403          	ld	s0,8(sp)
    80004058:	01010113          	addi	sp,sp,16
    8000405c:	00008067          	ret
    80004060:	0000006f          	j	80004060 <uartputc_sync+0x40>

0000000080004064 <uartstart>:
    80004064:	ff010113          	addi	sp,sp,-16
    80004068:	00813423          	sd	s0,8(sp)
    8000406c:	01010413          	addi	s0,sp,16
    80004070:	00002617          	auipc	a2,0x2
    80004074:	0a060613          	addi	a2,a2,160 # 80006110 <uart_tx_r>
    80004078:	00002517          	auipc	a0,0x2
    8000407c:	0a050513          	addi	a0,a0,160 # 80006118 <uart_tx_w>
    80004080:	00063783          	ld	a5,0(a2)
    80004084:	00053703          	ld	a4,0(a0)
    80004088:	04f70263          	beq	a4,a5,800040cc <uartstart+0x68>
    8000408c:	100005b7          	lui	a1,0x10000
    80004090:	0000b817          	auipc	a6,0xb
    80004094:	33080813          	addi	a6,a6,816 # 8000f3c0 <uart_tx_buf>
    80004098:	01c0006f          	j	800040b4 <uartstart+0x50>
    8000409c:	0006c703          	lbu	a4,0(a3)
    800040a0:	00f63023          	sd	a5,0(a2)
    800040a4:	00e58023          	sb	a4,0(a1) # 10000000 <offsetTCBregisters+0xffffff0>
    800040a8:	00063783          	ld	a5,0(a2)
    800040ac:	00053703          	ld	a4,0(a0)
    800040b0:	00f70e63          	beq	a4,a5,800040cc <uartstart+0x68>
    800040b4:	01f7f713          	andi	a4,a5,31
    800040b8:	00e806b3          	add	a3,a6,a4
    800040bc:	0055c703          	lbu	a4,5(a1)
    800040c0:	00178793          	addi	a5,a5,1
    800040c4:	02077713          	andi	a4,a4,32
    800040c8:	fc071ae3          	bnez	a4,8000409c <uartstart+0x38>
    800040cc:	00813403          	ld	s0,8(sp)
    800040d0:	01010113          	addi	sp,sp,16
    800040d4:	00008067          	ret

00000000800040d8 <uartgetc>:
    800040d8:	ff010113          	addi	sp,sp,-16
    800040dc:	00813423          	sd	s0,8(sp)
    800040e0:	01010413          	addi	s0,sp,16
    800040e4:	10000737          	lui	a4,0x10000
    800040e8:	00574783          	lbu	a5,5(a4) # 10000005 <offsetTCBregisters+0xffffff5>
    800040ec:	0017f793          	andi	a5,a5,1
    800040f0:	00078c63          	beqz	a5,80004108 <uartgetc+0x30>
    800040f4:	00074503          	lbu	a0,0(a4)
    800040f8:	0ff57513          	andi	a0,a0,255
    800040fc:	00813403          	ld	s0,8(sp)
    80004100:	01010113          	addi	sp,sp,16
    80004104:	00008067          	ret
    80004108:	fff00513          	li	a0,-1
    8000410c:	ff1ff06f          	j	800040fc <uartgetc+0x24>

0000000080004110 <uartintr>:
    80004110:	100007b7          	lui	a5,0x10000
    80004114:	0057c783          	lbu	a5,5(a5) # 10000005 <offsetTCBregisters+0xffffff5>
    80004118:	0017f793          	andi	a5,a5,1
    8000411c:	0a078463          	beqz	a5,800041c4 <uartintr+0xb4>
    80004120:	fe010113          	addi	sp,sp,-32
    80004124:	00813823          	sd	s0,16(sp)
    80004128:	00913423          	sd	s1,8(sp)
    8000412c:	00113c23          	sd	ra,24(sp)
    80004130:	02010413          	addi	s0,sp,32
    80004134:	100004b7          	lui	s1,0x10000
    80004138:	0004c503          	lbu	a0,0(s1) # 10000000 <offsetTCBregisters+0xffffff0>
    8000413c:	0ff57513          	andi	a0,a0,255
    80004140:	fffff097          	auipc	ra,0xfffff
    80004144:	534080e7          	jalr	1332(ra) # 80003674 <consoleintr>
    80004148:	0054c783          	lbu	a5,5(s1)
    8000414c:	0017f793          	andi	a5,a5,1
    80004150:	fe0794e3          	bnez	a5,80004138 <uartintr+0x28>
    80004154:	00002617          	auipc	a2,0x2
    80004158:	fbc60613          	addi	a2,a2,-68 # 80006110 <uart_tx_r>
    8000415c:	00002517          	auipc	a0,0x2
    80004160:	fbc50513          	addi	a0,a0,-68 # 80006118 <uart_tx_w>
    80004164:	00063783          	ld	a5,0(a2)
    80004168:	00053703          	ld	a4,0(a0)
    8000416c:	04f70263          	beq	a4,a5,800041b0 <uartintr+0xa0>
    80004170:	100005b7          	lui	a1,0x10000
    80004174:	0000b817          	auipc	a6,0xb
    80004178:	24c80813          	addi	a6,a6,588 # 8000f3c0 <uart_tx_buf>
    8000417c:	01c0006f          	j	80004198 <uartintr+0x88>
    80004180:	0006c703          	lbu	a4,0(a3)
    80004184:	00f63023          	sd	a5,0(a2)
    80004188:	00e58023          	sb	a4,0(a1) # 10000000 <offsetTCBregisters+0xffffff0>
    8000418c:	00063783          	ld	a5,0(a2)
    80004190:	00053703          	ld	a4,0(a0)
    80004194:	00f70e63          	beq	a4,a5,800041b0 <uartintr+0xa0>
    80004198:	01f7f713          	andi	a4,a5,31
    8000419c:	00e806b3          	add	a3,a6,a4
    800041a0:	0055c703          	lbu	a4,5(a1)
    800041a4:	00178793          	addi	a5,a5,1
    800041a8:	02077713          	andi	a4,a4,32
    800041ac:	fc071ae3          	bnez	a4,80004180 <uartintr+0x70>
    800041b0:	01813083          	ld	ra,24(sp)
    800041b4:	01013403          	ld	s0,16(sp)
    800041b8:	00813483          	ld	s1,8(sp)
    800041bc:	02010113          	addi	sp,sp,32
    800041c0:	00008067          	ret
    800041c4:	00002617          	auipc	a2,0x2
    800041c8:	f4c60613          	addi	a2,a2,-180 # 80006110 <uart_tx_r>
    800041cc:	00002517          	auipc	a0,0x2
    800041d0:	f4c50513          	addi	a0,a0,-180 # 80006118 <uart_tx_w>
    800041d4:	00063783          	ld	a5,0(a2)
    800041d8:	00053703          	ld	a4,0(a0)
    800041dc:	04f70263          	beq	a4,a5,80004220 <uartintr+0x110>
    800041e0:	100005b7          	lui	a1,0x10000
    800041e4:	0000b817          	auipc	a6,0xb
    800041e8:	1dc80813          	addi	a6,a6,476 # 8000f3c0 <uart_tx_buf>
    800041ec:	01c0006f          	j	80004208 <uartintr+0xf8>
    800041f0:	0006c703          	lbu	a4,0(a3)
    800041f4:	00f63023          	sd	a5,0(a2)
    800041f8:	00e58023          	sb	a4,0(a1) # 10000000 <offsetTCBregisters+0xffffff0>
    800041fc:	00063783          	ld	a5,0(a2)
    80004200:	00053703          	ld	a4,0(a0)
    80004204:	02f70063          	beq	a4,a5,80004224 <uartintr+0x114>
    80004208:	01f7f713          	andi	a4,a5,31
    8000420c:	00e806b3          	add	a3,a6,a4
    80004210:	0055c703          	lbu	a4,5(a1)
    80004214:	00178793          	addi	a5,a5,1
    80004218:	02077713          	andi	a4,a4,32
    8000421c:	fc071ae3          	bnez	a4,800041f0 <uartintr+0xe0>
    80004220:	00008067          	ret
    80004224:	00008067          	ret

0000000080004228 <kinit>:
    80004228:	fc010113          	addi	sp,sp,-64
    8000422c:	02913423          	sd	s1,40(sp)
    80004230:	fffff7b7          	lui	a5,0xfffff
    80004234:	0000c497          	auipc	s1,0xc
    80004238:	1ab48493          	addi	s1,s1,427 # 800103df <end+0xfff>
    8000423c:	02813823          	sd	s0,48(sp)
    80004240:	01313c23          	sd	s3,24(sp)
    80004244:	00f4f4b3          	and	s1,s1,a5
    80004248:	02113c23          	sd	ra,56(sp)
    8000424c:	03213023          	sd	s2,32(sp)
    80004250:	01413823          	sd	s4,16(sp)
    80004254:	01513423          	sd	s5,8(sp)
    80004258:	04010413          	addi	s0,sp,64
    8000425c:	000017b7          	lui	a5,0x1
    80004260:	01100993          	li	s3,17
    80004264:	00f487b3          	add	a5,s1,a5
    80004268:	01b99993          	slli	s3,s3,0x1b
    8000426c:	06f9e063          	bltu	s3,a5,800042cc <kinit+0xa4>
    80004270:	0000ba97          	auipc	s5,0xb
    80004274:	170a8a93          	addi	s5,s5,368 # 8000f3e0 <end>
    80004278:	0754ec63          	bltu	s1,s5,800042f0 <kinit+0xc8>
    8000427c:	0734fa63          	bgeu	s1,s3,800042f0 <kinit+0xc8>
    80004280:	00088a37          	lui	s4,0x88
    80004284:	fffa0a13          	addi	s4,s4,-1 # 87fff <offsetTCBregisters+0x87fef>
    80004288:	00002917          	auipc	s2,0x2
    8000428c:	e9890913          	addi	s2,s2,-360 # 80006120 <kmem>
    80004290:	00ca1a13          	slli	s4,s4,0xc
    80004294:	0140006f          	j	800042a8 <kinit+0x80>
    80004298:	000017b7          	lui	a5,0x1
    8000429c:	00f484b3          	add	s1,s1,a5
    800042a0:	0554e863          	bltu	s1,s5,800042f0 <kinit+0xc8>
    800042a4:	0534f663          	bgeu	s1,s3,800042f0 <kinit+0xc8>
    800042a8:	00001637          	lui	a2,0x1
    800042ac:	00100593          	li	a1,1
    800042b0:	00048513          	mv	a0,s1
    800042b4:	00000097          	auipc	ra,0x0
    800042b8:	5e4080e7          	jalr	1508(ra) # 80004898 <__memset>
    800042bc:	00093783          	ld	a5,0(s2)
    800042c0:	00f4b023          	sd	a5,0(s1)
    800042c4:	00993023          	sd	s1,0(s2)
    800042c8:	fd4498e3          	bne	s1,s4,80004298 <kinit+0x70>
    800042cc:	03813083          	ld	ra,56(sp)
    800042d0:	03013403          	ld	s0,48(sp)
    800042d4:	02813483          	ld	s1,40(sp)
    800042d8:	02013903          	ld	s2,32(sp)
    800042dc:	01813983          	ld	s3,24(sp)
    800042e0:	01013a03          	ld	s4,16(sp)
    800042e4:	00813a83          	ld	s5,8(sp)
    800042e8:	04010113          	addi	sp,sp,64
    800042ec:	00008067          	ret
    800042f0:	00001517          	auipc	a0,0x1
    800042f4:	fa050513          	addi	a0,a0,-96 # 80005290 <digits+0x18>
    800042f8:	fffff097          	auipc	ra,0xfffff
    800042fc:	4b4080e7          	jalr	1204(ra) # 800037ac <panic>

0000000080004300 <freerange>:
    80004300:	fc010113          	addi	sp,sp,-64
    80004304:	000017b7          	lui	a5,0x1
    80004308:	02913423          	sd	s1,40(sp)
    8000430c:	fff78493          	addi	s1,a5,-1 # fff <offsetTCBregisters+0xfef>
    80004310:	009504b3          	add	s1,a0,s1
    80004314:	fffff537          	lui	a0,0xfffff
    80004318:	02813823          	sd	s0,48(sp)
    8000431c:	02113c23          	sd	ra,56(sp)
    80004320:	03213023          	sd	s2,32(sp)
    80004324:	01313c23          	sd	s3,24(sp)
    80004328:	01413823          	sd	s4,16(sp)
    8000432c:	01513423          	sd	s5,8(sp)
    80004330:	01613023          	sd	s6,0(sp)
    80004334:	04010413          	addi	s0,sp,64
    80004338:	00a4f4b3          	and	s1,s1,a0
    8000433c:	00f487b3          	add	a5,s1,a5
    80004340:	06f5e463          	bltu	a1,a5,800043a8 <freerange+0xa8>
    80004344:	0000ba97          	auipc	s5,0xb
    80004348:	09ca8a93          	addi	s5,s5,156 # 8000f3e0 <end>
    8000434c:	0954e263          	bltu	s1,s5,800043d0 <freerange+0xd0>
    80004350:	01100993          	li	s3,17
    80004354:	01b99993          	slli	s3,s3,0x1b
    80004358:	0734fc63          	bgeu	s1,s3,800043d0 <freerange+0xd0>
    8000435c:	00058a13          	mv	s4,a1
    80004360:	00002917          	auipc	s2,0x2
    80004364:	dc090913          	addi	s2,s2,-576 # 80006120 <kmem>
    80004368:	00002b37          	lui	s6,0x2
    8000436c:	0140006f          	j	80004380 <freerange+0x80>
    80004370:	000017b7          	lui	a5,0x1
    80004374:	00f484b3          	add	s1,s1,a5
    80004378:	0554ec63          	bltu	s1,s5,800043d0 <freerange+0xd0>
    8000437c:	0534fa63          	bgeu	s1,s3,800043d0 <freerange+0xd0>
    80004380:	00001637          	lui	a2,0x1
    80004384:	00100593          	li	a1,1
    80004388:	00048513          	mv	a0,s1
    8000438c:	00000097          	auipc	ra,0x0
    80004390:	50c080e7          	jalr	1292(ra) # 80004898 <__memset>
    80004394:	00093703          	ld	a4,0(s2)
    80004398:	016487b3          	add	a5,s1,s6
    8000439c:	00e4b023          	sd	a4,0(s1)
    800043a0:	00993023          	sd	s1,0(s2)
    800043a4:	fcfa76e3          	bgeu	s4,a5,80004370 <freerange+0x70>
    800043a8:	03813083          	ld	ra,56(sp)
    800043ac:	03013403          	ld	s0,48(sp)
    800043b0:	02813483          	ld	s1,40(sp)
    800043b4:	02013903          	ld	s2,32(sp)
    800043b8:	01813983          	ld	s3,24(sp)
    800043bc:	01013a03          	ld	s4,16(sp)
    800043c0:	00813a83          	ld	s5,8(sp)
    800043c4:	00013b03          	ld	s6,0(sp)
    800043c8:	04010113          	addi	sp,sp,64
    800043cc:	00008067          	ret
    800043d0:	00001517          	auipc	a0,0x1
    800043d4:	ec050513          	addi	a0,a0,-320 # 80005290 <digits+0x18>
    800043d8:	fffff097          	auipc	ra,0xfffff
    800043dc:	3d4080e7          	jalr	980(ra) # 800037ac <panic>

00000000800043e0 <kfree>:
    800043e0:	fe010113          	addi	sp,sp,-32
    800043e4:	00813823          	sd	s0,16(sp)
    800043e8:	00113c23          	sd	ra,24(sp)
    800043ec:	00913423          	sd	s1,8(sp)
    800043f0:	02010413          	addi	s0,sp,32
    800043f4:	03451793          	slli	a5,a0,0x34
    800043f8:	04079c63          	bnez	a5,80004450 <kfree+0x70>
    800043fc:	0000b797          	auipc	a5,0xb
    80004400:	fe478793          	addi	a5,a5,-28 # 8000f3e0 <end>
    80004404:	00050493          	mv	s1,a0
    80004408:	04f56463          	bltu	a0,a5,80004450 <kfree+0x70>
    8000440c:	01100793          	li	a5,17
    80004410:	01b79793          	slli	a5,a5,0x1b
    80004414:	02f57e63          	bgeu	a0,a5,80004450 <kfree+0x70>
    80004418:	00001637          	lui	a2,0x1
    8000441c:	00100593          	li	a1,1
    80004420:	00000097          	auipc	ra,0x0
    80004424:	478080e7          	jalr	1144(ra) # 80004898 <__memset>
    80004428:	00002797          	auipc	a5,0x2
    8000442c:	cf878793          	addi	a5,a5,-776 # 80006120 <kmem>
    80004430:	0007b703          	ld	a4,0(a5)
    80004434:	01813083          	ld	ra,24(sp)
    80004438:	01013403          	ld	s0,16(sp)
    8000443c:	00e4b023          	sd	a4,0(s1)
    80004440:	0097b023          	sd	s1,0(a5)
    80004444:	00813483          	ld	s1,8(sp)
    80004448:	02010113          	addi	sp,sp,32
    8000444c:	00008067          	ret
    80004450:	00001517          	auipc	a0,0x1
    80004454:	e4050513          	addi	a0,a0,-448 # 80005290 <digits+0x18>
    80004458:	fffff097          	auipc	ra,0xfffff
    8000445c:	354080e7          	jalr	852(ra) # 800037ac <panic>

0000000080004460 <kalloc>:
    80004460:	fe010113          	addi	sp,sp,-32
    80004464:	00813823          	sd	s0,16(sp)
    80004468:	00913423          	sd	s1,8(sp)
    8000446c:	00113c23          	sd	ra,24(sp)
    80004470:	02010413          	addi	s0,sp,32
    80004474:	00002797          	auipc	a5,0x2
    80004478:	cac78793          	addi	a5,a5,-852 # 80006120 <kmem>
    8000447c:	0007b483          	ld	s1,0(a5)
    80004480:	02048063          	beqz	s1,800044a0 <kalloc+0x40>
    80004484:	0004b703          	ld	a4,0(s1)
    80004488:	00001637          	lui	a2,0x1
    8000448c:	00500593          	li	a1,5
    80004490:	00048513          	mv	a0,s1
    80004494:	00e7b023          	sd	a4,0(a5)
    80004498:	00000097          	auipc	ra,0x0
    8000449c:	400080e7          	jalr	1024(ra) # 80004898 <__memset>
    800044a0:	01813083          	ld	ra,24(sp)
    800044a4:	01013403          	ld	s0,16(sp)
    800044a8:	00048513          	mv	a0,s1
    800044ac:	00813483          	ld	s1,8(sp)
    800044b0:	02010113          	addi	sp,sp,32
    800044b4:	00008067          	ret

00000000800044b8 <initlock>:
    800044b8:	ff010113          	addi	sp,sp,-16
    800044bc:	00813423          	sd	s0,8(sp)
    800044c0:	01010413          	addi	s0,sp,16
    800044c4:	00813403          	ld	s0,8(sp)
    800044c8:	00b53423          	sd	a1,8(a0)
    800044cc:	00052023          	sw	zero,0(a0)
    800044d0:	00053823          	sd	zero,16(a0)
    800044d4:	01010113          	addi	sp,sp,16
    800044d8:	00008067          	ret

00000000800044dc <acquire>:
    800044dc:	fe010113          	addi	sp,sp,-32
    800044e0:	00813823          	sd	s0,16(sp)
    800044e4:	00913423          	sd	s1,8(sp)
    800044e8:	00113c23          	sd	ra,24(sp)
    800044ec:	01213023          	sd	s2,0(sp)
    800044f0:	02010413          	addi	s0,sp,32
    800044f4:	00050493          	mv	s1,a0
    800044f8:	10002973          	csrr	s2,sstatus
    800044fc:	100027f3          	csrr	a5,sstatus
    80004500:	ffd7f793          	andi	a5,a5,-3
    80004504:	10079073          	csrw	sstatus,a5
    80004508:	fffff097          	auipc	ra,0xfffff
    8000450c:	8e8080e7          	jalr	-1816(ra) # 80002df0 <mycpu>
    80004510:	07852783          	lw	a5,120(a0)
    80004514:	06078e63          	beqz	a5,80004590 <acquire+0xb4>
    80004518:	fffff097          	auipc	ra,0xfffff
    8000451c:	8d8080e7          	jalr	-1832(ra) # 80002df0 <mycpu>
    80004520:	07852783          	lw	a5,120(a0)
    80004524:	0004a703          	lw	a4,0(s1)
    80004528:	0017879b          	addiw	a5,a5,1
    8000452c:	06f52c23          	sw	a5,120(a0)
    80004530:	04071063          	bnez	a4,80004570 <acquire+0x94>
    80004534:	00100713          	li	a4,1
    80004538:	00070793          	mv	a5,a4
    8000453c:	0cf4a7af          	amoswap.w.aq	a5,a5,(s1)
    80004540:	0007879b          	sext.w	a5,a5
    80004544:	fe079ae3          	bnez	a5,80004538 <acquire+0x5c>
    80004548:	0ff0000f          	fence
    8000454c:	fffff097          	auipc	ra,0xfffff
    80004550:	8a4080e7          	jalr	-1884(ra) # 80002df0 <mycpu>
    80004554:	01813083          	ld	ra,24(sp)
    80004558:	01013403          	ld	s0,16(sp)
    8000455c:	00a4b823          	sd	a0,16(s1)
    80004560:	00013903          	ld	s2,0(sp)
    80004564:	00813483          	ld	s1,8(sp)
    80004568:	02010113          	addi	sp,sp,32
    8000456c:	00008067          	ret
    80004570:	0104b903          	ld	s2,16(s1)
    80004574:	fffff097          	auipc	ra,0xfffff
    80004578:	87c080e7          	jalr	-1924(ra) # 80002df0 <mycpu>
    8000457c:	faa91ce3          	bne	s2,a0,80004534 <acquire+0x58>
    80004580:	00001517          	auipc	a0,0x1
    80004584:	d1850513          	addi	a0,a0,-744 # 80005298 <digits+0x20>
    80004588:	fffff097          	auipc	ra,0xfffff
    8000458c:	224080e7          	jalr	548(ra) # 800037ac <panic>
    80004590:	00195913          	srli	s2,s2,0x1
    80004594:	fffff097          	auipc	ra,0xfffff
    80004598:	85c080e7          	jalr	-1956(ra) # 80002df0 <mycpu>
    8000459c:	00197913          	andi	s2,s2,1
    800045a0:	07252e23          	sw	s2,124(a0)
    800045a4:	f75ff06f          	j	80004518 <acquire+0x3c>

00000000800045a8 <release>:
    800045a8:	fe010113          	addi	sp,sp,-32
    800045ac:	00813823          	sd	s0,16(sp)
    800045b0:	00113c23          	sd	ra,24(sp)
    800045b4:	00913423          	sd	s1,8(sp)
    800045b8:	01213023          	sd	s2,0(sp)
    800045bc:	02010413          	addi	s0,sp,32
    800045c0:	00052783          	lw	a5,0(a0)
    800045c4:	00079a63          	bnez	a5,800045d8 <release+0x30>
    800045c8:	00001517          	auipc	a0,0x1
    800045cc:	cd850513          	addi	a0,a0,-808 # 800052a0 <digits+0x28>
    800045d0:	fffff097          	auipc	ra,0xfffff
    800045d4:	1dc080e7          	jalr	476(ra) # 800037ac <panic>
    800045d8:	01053903          	ld	s2,16(a0)
    800045dc:	00050493          	mv	s1,a0
    800045e0:	fffff097          	auipc	ra,0xfffff
    800045e4:	810080e7          	jalr	-2032(ra) # 80002df0 <mycpu>
    800045e8:	fea910e3          	bne	s2,a0,800045c8 <release+0x20>
    800045ec:	0004b823          	sd	zero,16(s1)
    800045f0:	0ff0000f          	fence
    800045f4:	0f50000f          	fence	iorw,ow
    800045f8:	0804a02f          	amoswap.w	zero,zero,(s1)
    800045fc:	ffffe097          	auipc	ra,0xffffe
    80004600:	7f4080e7          	jalr	2036(ra) # 80002df0 <mycpu>
    80004604:	100027f3          	csrr	a5,sstatus
    80004608:	0027f793          	andi	a5,a5,2
    8000460c:	04079a63          	bnez	a5,80004660 <release+0xb8>
    80004610:	07852783          	lw	a5,120(a0)
    80004614:	02f05e63          	blez	a5,80004650 <release+0xa8>
    80004618:	fff7871b          	addiw	a4,a5,-1
    8000461c:	06e52c23          	sw	a4,120(a0)
    80004620:	00071c63          	bnez	a4,80004638 <release+0x90>
    80004624:	07c52783          	lw	a5,124(a0)
    80004628:	00078863          	beqz	a5,80004638 <release+0x90>
    8000462c:	100027f3          	csrr	a5,sstatus
    80004630:	0027e793          	ori	a5,a5,2
    80004634:	10079073          	csrw	sstatus,a5
    80004638:	01813083          	ld	ra,24(sp)
    8000463c:	01013403          	ld	s0,16(sp)
    80004640:	00813483          	ld	s1,8(sp)
    80004644:	00013903          	ld	s2,0(sp)
    80004648:	02010113          	addi	sp,sp,32
    8000464c:	00008067          	ret
    80004650:	00001517          	auipc	a0,0x1
    80004654:	c7050513          	addi	a0,a0,-912 # 800052c0 <digits+0x48>
    80004658:	fffff097          	auipc	ra,0xfffff
    8000465c:	154080e7          	jalr	340(ra) # 800037ac <panic>
    80004660:	00001517          	auipc	a0,0x1
    80004664:	c4850513          	addi	a0,a0,-952 # 800052a8 <digits+0x30>
    80004668:	fffff097          	auipc	ra,0xfffff
    8000466c:	144080e7          	jalr	324(ra) # 800037ac <panic>

0000000080004670 <holding>:
    80004670:	00052783          	lw	a5,0(a0)
    80004674:	00079663          	bnez	a5,80004680 <holding+0x10>
    80004678:	00000513          	li	a0,0
    8000467c:	00008067          	ret
    80004680:	fe010113          	addi	sp,sp,-32
    80004684:	00813823          	sd	s0,16(sp)
    80004688:	00913423          	sd	s1,8(sp)
    8000468c:	00113c23          	sd	ra,24(sp)
    80004690:	02010413          	addi	s0,sp,32
    80004694:	01053483          	ld	s1,16(a0)
    80004698:	ffffe097          	auipc	ra,0xffffe
    8000469c:	758080e7          	jalr	1880(ra) # 80002df0 <mycpu>
    800046a0:	01813083          	ld	ra,24(sp)
    800046a4:	01013403          	ld	s0,16(sp)
    800046a8:	40a48533          	sub	a0,s1,a0
    800046ac:	00153513          	seqz	a0,a0
    800046b0:	00813483          	ld	s1,8(sp)
    800046b4:	02010113          	addi	sp,sp,32
    800046b8:	00008067          	ret

00000000800046bc <push_off>:
    800046bc:	fe010113          	addi	sp,sp,-32
    800046c0:	00813823          	sd	s0,16(sp)
    800046c4:	00113c23          	sd	ra,24(sp)
    800046c8:	00913423          	sd	s1,8(sp)
    800046cc:	02010413          	addi	s0,sp,32
    800046d0:	100024f3          	csrr	s1,sstatus
    800046d4:	100027f3          	csrr	a5,sstatus
    800046d8:	ffd7f793          	andi	a5,a5,-3
    800046dc:	10079073          	csrw	sstatus,a5
    800046e0:	ffffe097          	auipc	ra,0xffffe
    800046e4:	710080e7          	jalr	1808(ra) # 80002df0 <mycpu>
    800046e8:	07852783          	lw	a5,120(a0)
    800046ec:	02078663          	beqz	a5,80004718 <push_off+0x5c>
    800046f0:	ffffe097          	auipc	ra,0xffffe
    800046f4:	700080e7          	jalr	1792(ra) # 80002df0 <mycpu>
    800046f8:	07852783          	lw	a5,120(a0)
    800046fc:	01813083          	ld	ra,24(sp)
    80004700:	01013403          	ld	s0,16(sp)
    80004704:	0017879b          	addiw	a5,a5,1
    80004708:	06f52c23          	sw	a5,120(a0)
    8000470c:	00813483          	ld	s1,8(sp)
    80004710:	02010113          	addi	sp,sp,32
    80004714:	00008067          	ret
    80004718:	0014d493          	srli	s1,s1,0x1
    8000471c:	ffffe097          	auipc	ra,0xffffe
    80004720:	6d4080e7          	jalr	1748(ra) # 80002df0 <mycpu>
    80004724:	0014f493          	andi	s1,s1,1
    80004728:	06952e23          	sw	s1,124(a0)
    8000472c:	fc5ff06f          	j	800046f0 <push_off+0x34>

0000000080004730 <pop_off>:
    80004730:	ff010113          	addi	sp,sp,-16
    80004734:	00813023          	sd	s0,0(sp)
    80004738:	00113423          	sd	ra,8(sp)
    8000473c:	01010413          	addi	s0,sp,16
    80004740:	ffffe097          	auipc	ra,0xffffe
    80004744:	6b0080e7          	jalr	1712(ra) # 80002df0 <mycpu>
    80004748:	100027f3          	csrr	a5,sstatus
    8000474c:	0027f793          	andi	a5,a5,2
    80004750:	04079663          	bnez	a5,8000479c <pop_off+0x6c>
    80004754:	07852783          	lw	a5,120(a0)
    80004758:	02f05a63          	blez	a5,8000478c <pop_off+0x5c>
    8000475c:	fff7871b          	addiw	a4,a5,-1
    80004760:	06e52c23          	sw	a4,120(a0)
    80004764:	00071c63          	bnez	a4,8000477c <pop_off+0x4c>
    80004768:	07c52783          	lw	a5,124(a0)
    8000476c:	00078863          	beqz	a5,8000477c <pop_off+0x4c>
    80004770:	100027f3          	csrr	a5,sstatus
    80004774:	0027e793          	ori	a5,a5,2
    80004778:	10079073          	csrw	sstatus,a5
    8000477c:	00813083          	ld	ra,8(sp)
    80004780:	00013403          	ld	s0,0(sp)
    80004784:	01010113          	addi	sp,sp,16
    80004788:	00008067          	ret
    8000478c:	00001517          	auipc	a0,0x1
    80004790:	b3450513          	addi	a0,a0,-1228 # 800052c0 <digits+0x48>
    80004794:	fffff097          	auipc	ra,0xfffff
    80004798:	018080e7          	jalr	24(ra) # 800037ac <panic>
    8000479c:	00001517          	auipc	a0,0x1
    800047a0:	b0c50513          	addi	a0,a0,-1268 # 800052a8 <digits+0x30>
    800047a4:	fffff097          	auipc	ra,0xfffff
    800047a8:	008080e7          	jalr	8(ra) # 800037ac <panic>

00000000800047ac <push_on>:
    800047ac:	fe010113          	addi	sp,sp,-32
    800047b0:	00813823          	sd	s0,16(sp)
    800047b4:	00113c23          	sd	ra,24(sp)
    800047b8:	00913423          	sd	s1,8(sp)
    800047bc:	02010413          	addi	s0,sp,32
    800047c0:	100024f3          	csrr	s1,sstatus
    800047c4:	100027f3          	csrr	a5,sstatus
    800047c8:	0027e793          	ori	a5,a5,2
    800047cc:	10079073          	csrw	sstatus,a5
    800047d0:	ffffe097          	auipc	ra,0xffffe
    800047d4:	620080e7          	jalr	1568(ra) # 80002df0 <mycpu>
    800047d8:	07852783          	lw	a5,120(a0)
    800047dc:	02078663          	beqz	a5,80004808 <push_on+0x5c>
    800047e0:	ffffe097          	auipc	ra,0xffffe
    800047e4:	610080e7          	jalr	1552(ra) # 80002df0 <mycpu>
    800047e8:	07852783          	lw	a5,120(a0)
    800047ec:	01813083          	ld	ra,24(sp)
    800047f0:	01013403          	ld	s0,16(sp)
    800047f4:	0017879b          	addiw	a5,a5,1
    800047f8:	06f52c23          	sw	a5,120(a0)
    800047fc:	00813483          	ld	s1,8(sp)
    80004800:	02010113          	addi	sp,sp,32
    80004804:	00008067          	ret
    80004808:	0014d493          	srli	s1,s1,0x1
    8000480c:	ffffe097          	auipc	ra,0xffffe
    80004810:	5e4080e7          	jalr	1508(ra) # 80002df0 <mycpu>
    80004814:	0014f493          	andi	s1,s1,1
    80004818:	06952e23          	sw	s1,124(a0)
    8000481c:	fc5ff06f          	j	800047e0 <push_on+0x34>

0000000080004820 <pop_on>:
    80004820:	ff010113          	addi	sp,sp,-16
    80004824:	00813023          	sd	s0,0(sp)
    80004828:	00113423          	sd	ra,8(sp)
    8000482c:	01010413          	addi	s0,sp,16
    80004830:	ffffe097          	auipc	ra,0xffffe
    80004834:	5c0080e7          	jalr	1472(ra) # 80002df0 <mycpu>
    80004838:	100027f3          	csrr	a5,sstatus
    8000483c:	0027f793          	andi	a5,a5,2
    80004840:	04078463          	beqz	a5,80004888 <pop_on+0x68>
    80004844:	07852783          	lw	a5,120(a0)
    80004848:	02f05863          	blez	a5,80004878 <pop_on+0x58>
    8000484c:	fff7879b          	addiw	a5,a5,-1
    80004850:	06f52c23          	sw	a5,120(a0)
    80004854:	07853783          	ld	a5,120(a0)
    80004858:	00079863          	bnez	a5,80004868 <pop_on+0x48>
    8000485c:	100027f3          	csrr	a5,sstatus
    80004860:	ffd7f793          	andi	a5,a5,-3
    80004864:	10079073          	csrw	sstatus,a5
    80004868:	00813083          	ld	ra,8(sp)
    8000486c:	00013403          	ld	s0,0(sp)
    80004870:	01010113          	addi	sp,sp,16
    80004874:	00008067          	ret
    80004878:	00001517          	auipc	a0,0x1
    8000487c:	a7050513          	addi	a0,a0,-1424 # 800052e8 <digits+0x70>
    80004880:	fffff097          	auipc	ra,0xfffff
    80004884:	f2c080e7          	jalr	-212(ra) # 800037ac <panic>
    80004888:	00001517          	auipc	a0,0x1
    8000488c:	a4050513          	addi	a0,a0,-1472 # 800052c8 <digits+0x50>
    80004890:	fffff097          	auipc	ra,0xfffff
    80004894:	f1c080e7          	jalr	-228(ra) # 800037ac <panic>

0000000080004898 <__memset>:
    80004898:	ff010113          	addi	sp,sp,-16
    8000489c:	00813423          	sd	s0,8(sp)
    800048a0:	01010413          	addi	s0,sp,16
    800048a4:	1a060e63          	beqz	a2,80004a60 <__memset+0x1c8>
    800048a8:	40a007b3          	neg	a5,a0
    800048ac:	0077f793          	andi	a5,a5,7
    800048b0:	00778693          	addi	a3,a5,7
    800048b4:	00b00813          	li	a6,11
    800048b8:	0ff5f593          	andi	a1,a1,255
    800048bc:	fff6071b          	addiw	a4,a2,-1
    800048c0:	1b06e663          	bltu	a3,a6,80004a6c <__memset+0x1d4>
    800048c4:	1cd76463          	bltu	a4,a3,80004a8c <__memset+0x1f4>
    800048c8:	1a078e63          	beqz	a5,80004a84 <__memset+0x1ec>
    800048cc:	00b50023          	sb	a1,0(a0)
    800048d0:	00100713          	li	a4,1
    800048d4:	1ae78463          	beq	a5,a4,80004a7c <__memset+0x1e4>
    800048d8:	00b500a3          	sb	a1,1(a0)
    800048dc:	00200713          	li	a4,2
    800048e0:	1ae78a63          	beq	a5,a4,80004a94 <__memset+0x1fc>
    800048e4:	00b50123          	sb	a1,2(a0)
    800048e8:	00300713          	li	a4,3
    800048ec:	18e78463          	beq	a5,a4,80004a74 <__memset+0x1dc>
    800048f0:	00b501a3          	sb	a1,3(a0)
    800048f4:	00400713          	li	a4,4
    800048f8:	1ae78263          	beq	a5,a4,80004a9c <__memset+0x204>
    800048fc:	00b50223          	sb	a1,4(a0)
    80004900:	00500713          	li	a4,5
    80004904:	1ae78063          	beq	a5,a4,80004aa4 <__memset+0x20c>
    80004908:	00b502a3          	sb	a1,5(a0)
    8000490c:	00700713          	li	a4,7
    80004910:	18e79e63          	bne	a5,a4,80004aac <__memset+0x214>
    80004914:	00b50323          	sb	a1,6(a0)
    80004918:	00700e93          	li	t4,7
    8000491c:	00859713          	slli	a4,a1,0x8
    80004920:	00e5e733          	or	a4,a1,a4
    80004924:	01059e13          	slli	t3,a1,0x10
    80004928:	01c76e33          	or	t3,a4,t3
    8000492c:	01859313          	slli	t1,a1,0x18
    80004930:	006e6333          	or	t1,t3,t1
    80004934:	02059893          	slli	a7,a1,0x20
    80004938:	40f60e3b          	subw	t3,a2,a5
    8000493c:	011368b3          	or	a7,t1,a7
    80004940:	02859813          	slli	a6,a1,0x28
    80004944:	0108e833          	or	a6,a7,a6
    80004948:	03059693          	slli	a3,a1,0x30
    8000494c:	003e589b          	srliw	a7,t3,0x3
    80004950:	00d866b3          	or	a3,a6,a3
    80004954:	03859713          	slli	a4,a1,0x38
    80004958:	00389813          	slli	a6,a7,0x3
    8000495c:	00f507b3          	add	a5,a0,a5
    80004960:	00e6e733          	or	a4,a3,a4
    80004964:	000e089b          	sext.w	a7,t3
    80004968:	00f806b3          	add	a3,a6,a5
    8000496c:	00e7b023          	sd	a4,0(a5)
    80004970:	00878793          	addi	a5,a5,8
    80004974:	fed79ce3          	bne	a5,a3,8000496c <__memset+0xd4>
    80004978:	ff8e7793          	andi	a5,t3,-8
    8000497c:	0007871b          	sext.w	a4,a5
    80004980:	01d787bb          	addw	a5,a5,t4
    80004984:	0ce88e63          	beq	a7,a4,80004a60 <__memset+0x1c8>
    80004988:	00f50733          	add	a4,a0,a5
    8000498c:	00b70023          	sb	a1,0(a4)
    80004990:	0017871b          	addiw	a4,a5,1
    80004994:	0cc77663          	bgeu	a4,a2,80004a60 <__memset+0x1c8>
    80004998:	00e50733          	add	a4,a0,a4
    8000499c:	00b70023          	sb	a1,0(a4)
    800049a0:	0027871b          	addiw	a4,a5,2
    800049a4:	0ac77e63          	bgeu	a4,a2,80004a60 <__memset+0x1c8>
    800049a8:	00e50733          	add	a4,a0,a4
    800049ac:	00b70023          	sb	a1,0(a4)
    800049b0:	0037871b          	addiw	a4,a5,3
    800049b4:	0ac77663          	bgeu	a4,a2,80004a60 <__memset+0x1c8>
    800049b8:	00e50733          	add	a4,a0,a4
    800049bc:	00b70023          	sb	a1,0(a4)
    800049c0:	0047871b          	addiw	a4,a5,4
    800049c4:	08c77e63          	bgeu	a4,a2,80004a60 <__memset+0x1c8>
    800049c8:	00e50733          	add	a4,a0,a4
    800049cc:	00b70023          	sb	a1,0(a4)
    800049d0:	0057871b          	addiw	a4,a5,5
    800049d4:	08c77663          	bgeu	a4,a2,80004a60 <__memset+0x1c8>
    800049d8:	00e50733          	add	a4,a0,a4
    800049dc:	00b70023          	sb	a1,0(a4)
    800049e0:	0067871b          	addiw	a4,a5,6
    800049e4:	06c77e63          	bgeu	a4,a2,80004a60 <__memset+0x1c8>
    800049e8:	00e50733          	add	a4,a0,a4
    800049ec:	00b70023          	sb	a1,0(a4)
    800049f0:	0077871b          	addiw	a4,a5,7
    800049f4:	06c77663          	bgeu	a4,a2,80004a60 <__memset+0x1c8>
    800049f8:	00e50733          	add	a4,a0,a4
    800049fc:	00b70023          	sb	a1,0(a4)
    80004a00:	0087871b          	addiw	a4,a5,8
    80004a04:	04c77e63          	bgeu	a4,a2,80004a60 <__memset+0x1c8>
    80004a08:	00e50733          	add	a4,a0,a4
    80004a0c:	00b70023          	sb	a1,0(a4)
    80004a10:	0097871b          	addiw	a4,a5,9
    80004a14:	04c77663          	bgeu	a4,a2,80004a60 <__memset+0x1c8>
    80004a18:	00e50733          	add	a4,a0,a4
    80004a1c:	00b70023          	sb	a1,0(a4)
    80004a20:	00a7871b          	addiw	a4,a5,10
    80004a24:	02c77e63          	bgeu	a4,a2,80004a60 <__memset+0x1c8>
    80004a28:	00e50733          	add	a4,a0,a4
    80004a2c:	00b70023          	sb	a1,0(a4)
    80004a30:	00b7871b          	addiw	a4,a5,11
    80004a34:	02c77663          	bgeu	a4,a2,80004a60 <__memset+0x1c8>
    80004a38:	00e50733          	add	a4,a0,a4
    80004a3c:	00b70023          	sb	a1,0(a4)
    80004a40:	00c7871b          	addiw	a4,a5,12
    80004a44:	00c77e63          	bgeu	a4,a2,80004a60 <__memset+0x1c8>
    80004a48:	00e50733          	add	a4,a0,a4
    80004a4c:	00b70023          	sb	a1,0(a4)
    80004a50:	00d7879b          	addiw	a5,a5,13
    80004a54:	00c7f663          	bgeu	a5,a2,80004a60 <__memset+0x1c8>
    80004a58:	00f507b3          	add	a5,a0,a5
    80004a5c:	00b78023          	sb	a1,0(a5)
    80004a60:	00813403          	ld	s0,8(sp)
    80004a64:	01010113          	addi	sp,sp,16
    80004a68:	00008067          	ret
    80004a6c:	00b00693          	li	a3,11
    80004a70:	e55ff06f          	j	800048c4 <__memset+0x2c>
    80004a74:	00300e93          	li	t4,3
    80004a78:	ea5ff06f          	j	8000491c <__memset+0x84>
    80004a7c:	00100e93          	li	t4,1
    80004a80:	e9dff06f          	j	8000491c <__memset+0x84>
    80004a84:	00000e93          	li	t4,0
    80004a88:	e95ff06f          	j	8000491c <__memset+0x84>
    80004a8c:	00000793          	li	a5,0
    80004a90:	ef9ff06f          	j	80004988 <__memset+0xf0>
    80004a94:	00200e93          	li	t4,2
    80004a98:	e85ff06f          	j	8000491c <__memset+0x84>
    80004a9c:	00400e93          	li	t4,4
    80004aa0:	e7dff06f          	j	8000491c <__memset+0x84>
    80004aa4:	00500e93          	li	t4,5
    80004aa8:	e75ff06f          	j	8000491c <__memset+0x84>
    80004aac:	00600e93          	li	t4,6
    80004ab0:	e6dff06f          	j	8000491c <__memset+0x84>

0000000080004ab4 <__memmove>:
    80004ab4:	ff010113          	addi	sp,sp,-16
    80004ab8:	00813423          	sd	s0,8(sp)
    80004abc:	01010413          	addi	s0,sp,16
    80004ac0:	0e060863          	beqz	a2,80004bb0 <__memmove+0xfc>
    80004ac4:	fff6069b          	addiw	a3,a2,-1
    80004ac8:	0006881b          	sext.w	a6,a3
    80004acc:	0ea5e863          	bltu	a1,a0,80004bbc <__memmove+0x108>
    80004ad0:	00758713          	addi	a4,a1,7
    80004ad4:	00a5e7b3          	or	a5,a1,a0
    80004ad8:	40a70733          	sub	a4,a4,a0
    80004adc:	0077f793          	andi	a5,a5,7
    80004ae0:	00f73713          	sltiu	a4,a4,15
    80004ae4:	00174713          	xori	a4,a4,1
    80004ae8:	0017b793          	seqz	a5,a5
    80004aec:	00e7f7b3          	and	a5,a5,a4
    80004af0:	10078863          	beqz	a5,80004c00 <__memmove+0x14c>
    80004af4:	00900793          	li	a5,9
    80004af8:	1107f463          	bgeu	a5,a6,80004c00 <__memmove+0x14c>
    80004afc:	0036581b          	srliw	a6,a2,0x3
    80004b00:	fff8081b          	addiw	a6,a6,-1
    80004b04:	02081813          	slli	a6,a6,0x20
    80004b08:	01d85893          	srli	a7,a6,0x1d
    80004b0c:	00858813          	addi	a6,a1,8
    80004b10:	00058793          	mv	a5,a1
    80004b14:	00050713          	mv	a4,a0
    80004b18:	01088833          	add	a6,a7,a6
    80004b1c:	0007b883          	ld	a7,0(a5)
    80004b20:	00878793          	addi	a5,a5,8
    80004b24:	00870713          	addi	a4,a4,8
    80004b28:	ff173c23          	sd	a7,-8(a4)
    80004b2c:	ff0798e3          	bne	a5,a6,80004b1c <__memmove+0x68>
    80004b30:	ff867713          	andi	a4,a2,-8
    80004b34:	02071793          	slli	a5,a4,0x20
    80004b38:	0207d793          	srli	a5,a5,0x20
    80004b3c:	00f585b3          	add	a1,a1,a5
    80004b40:	40e686bb          	subw	a3,a3,a4
    80004b44:	00f507b3          	add	a5,a0,a5
    80004b48:	06e60463          	beq	a2,a4,80004bb0 <__memmove+0xfc>
    80004b4c:	0005c703          	lbu	a4,0(a1)
    80004b50:	00e78023          	sb	a4,0(a5)
    80004b54:	04068e63          	beqz	a3,80004bb0 <__memmove+0xfc>
    80004b58:	0015c603          	lbu	a2,1(a1)
    80004b5c:	00100713          	li	a4,1
    80004b60:	00c780a3          	sb	a2,1(a5)
    80004b64:	04e68663          	beq	a3,a4,80004bb0 <__memmove+0xfc>
    80004b68:	0025c603          	lbu	a2,2(a1)
    80004b6c:	00200713          	li	a4,2
    80004b70:	00c78123          	sb	a2,2(a5)
    80004b74:	02e68e63          	beq	a3,a4,80004bb0 <__memmove+0xfc>
    80004b78:	0035c603          	lbu	a2,3(a1)
    80004b7c:	00300713          	li	a4,3
    80004b80:	00c781a3          	sb	a2,3(a5)
    80004b84:	02e68663          	beq	a3,a4,80004bb0 <__memmove+0xfc>
    80004b88:	0045c603          	lbu	a2,4(a1)
    80004b8c:	00400713          	li	a4,4
    80004b90:	00c78223          	sb	a2,4(a5)
    80004b94:	00e68e63          	beq	a3,a4,80004bb0 <__memmove+0xfc>
    80004b98:	0055c603          	lbu	a2,5(a1)
    80004b9c:	00500713          	li	a4,5
    80004ba0:	00c782a3          	sb	a2,5(a5)
    80004ba4:	00e68663          	beq	a3,a4,80004bb0 <__memmove+0xfc>
    80004ba8:	0065c703          	lbu	a4,6(a1)
    80004bac:	00e78323          	sb	a4,6(a5)
    80004bb0:	00813403          	ld	s0,8(sp)
    80004bb4:	01010113          	addi	sp,sp,16
    80004bb8:	00008067          	ret
    80004bbc:	02061713          	slli	a4,a2,0x20
    80004bc0:	02075713          	srli	a4,a4,0x20
    80004bc4:	00e587b3          	add	a5,a1,a4
    80004bc8:	f0f574e3          	bgeu	a0,a5,80004ad0 <__memmove+0x1c>
    80004bcc:	02069613          	slli	a2,a3,0x20
    80004bd0:	02065613          	srli	a2,a2,0x20
    80004bd4:	fff64613          	not	a2,a2
    80004bd8:	00e50733          	add	a4,a0,a4
    80004bdc:	00c78633          	add	a2,a5,a2
    80004be0:	fff7c683          	lbu	a3,-1(a5)
    80004be4:	fff78793          	addi	a5,a5,-1
    80004be8:	fff70713          	addi	a4,a4,-1
    80004bec:	00d70023          	sb	a3,0(a4)
    80004bf0:	fec798e3          	bne	a5,a2,80004be0 <__memmove+0x12c>
    80004bf4:	00813403          	ld	s0,8(sp)
    80004bf8:	01010113          	addi	sp,sp,16
    80004bfc:	00008067          	ret
    80004c00:	02069713          	slli	a4,a3,0x20
    80004c04:	02075713          	srli	a4,a4,0x20
    80004c08:	00170713          	addi	a4,a4,1
    80004c0c:	00e50733          	add	a4,a0,a4
    80004c10:	00050793          	mv	a5,a0
    80004c14:	0005c683          	lbu	a3,0(a1)
    80004c18:	00178793          	addi	a5,a5,1
    80004c1c:	00158593          	addi	a1,a1,1
    80004c20:	fed78fa3          	sb	a3,-1(a5)
    80004c24:	fee798e3          	bne	a5,a4,80004c14 <__memmove+0x160>
    80004c28:	f89ff06f          	j	80004bb0 <__memmove+0xfc>

0000000080004c2c <__putc>:
    80004c2c:	fe010113          	addi	sp,sp,-32
    80004c30:	00813823          	sd	s0,16(sp)
    80004c34:	00113c23          	sd	ra,24(sp)
    80004c38:	02010413          	addi	s0,sp,32
    80004c3c:	00050793          	mv	a5,a0
    80004c40:	fef40593          	addi	a1,s0,-17
    80004c44:	00100613          	li	a2,1
    80004c48:	00000513          	li	a0,0
    80004c4c:	fef407a3          	sb	a5,-17(s0)
    80004c50:	fffff097          	auipc	ra,0xfffff
    80004c54:	b3c080e7          	jalr	-1220(ra) # 8000378c <console_write>
    80004c58:	01813083          	ld	ra,24(sp)
    80004c5c:	01013403          	ld	s0,16(sp)
    80004c60:	02010113          	addi	sp,sp,32
    80004c64:	00008067          	ret

0000000080004c68 <__getc>:
    80004c68:	fe010113          	addi	sp,sp,-32
    80004c6c:	00813823          	sd	s0,16(sp)
    80004c70:	00113c23          	sd	ra,24(sp)
    80004c74:	02010413          	addi	s0,sp,32
    80004c78:	fe840593          	addi	a1,s0,-24
    80004c7c:	00100613          	li	a2,1
    80004c80:	00000513          	li	a0,0
    80004c84:	fffff097          	auipc	ra,0xfffff
    80004c88:	ae8080e7          	jalr	-1304(ra) # 8000376c <console_read>
    80004c8c:	fe844503          	lbu	a0,-24(s0)
    80004c90:	01813083          	ld	ra,24(sp)
    80004c94:	01013403          	ld	s0,16(sp)
    80004c98:	02010113          	addi	sp,sp,32
    80004c9c:	00008067          	ret

0000000080004ca0 <console_handler>:
    80004ca0:	fe010113          	addi	sp,sp,-32
    80004ca4:	00813823          	sd	s0,16(sp)
    80004ca8:	00113c23          	sd	ra,24(sp)
    80004cac:	00913423          	sd	s1,8(sp)
    80004cb0:	02010413          	addi	s0,sp,32
    80004cb4:	14202773          	csrr	a4,scause
    80004cb8:	100027f3          	csrr	a5,sstatus
    80004cbc:	0027f793          	andi	a5,a5,2
    80004cc0:	06079e63          	bnez	a5,80004d3c <console_handler+0x9c>
    80004cc4:	00074c63          	bltz	a4,80004cdc <console_handler+0x3c>
    80004cc8:	01813083          	ld	ra,24(sp)
    80004ccc:	01013403          	ld	s0,16(sp)
    80004cd0:	00813483          	ld	s1,8(sp)
    80004cd4:	02010113          	addi	sp,sp,32
    80004cd8:	00008067          	ret
    80004cdc:	0ff77713          	andi	a4,a4,255
    80004ce0:	00900793          	li	a5,9
    80004ce4:	fef712e3          	bne	a4,a5,80004cc8 <console_handler+0x28>
    80004ce8:	ffffe097          	auipc	ra,0xffffe
    80004cec:	6dc080e7          	jalr	1756(ra) # 800033c4 <plic_claim>
    80004cf0:	00a00793          	li	a5,10
    80004cf4:	00050493          	mv	s1,a0
    80004cf8:	02f50c63          	beq	a0,a5,80004d30 <console_handler+0x90>
    80004cfc:	fc0506e3          	beqz	a0,80004cc8 <console_handler+0x28>
    80004d00:	00050593          	mv	a1,a0
    80004d04:	00000517          	auipc	a0,0x0
    80004d08:	4ec50513          	addi	a0,a0,1260 # 800051f0 <CONSOLE_STATUS+0x1e0>
    80004d0c:	fffff097          	auipc	ra,0xfffff
    80004d10:	afc080e7          	jalr	-1284(ra) # 80003808 <__printf>
    80004d14:	01013403          	ld	s0,16(sp)
    80004d18:	01813083          	ld	ra,24(sp)
    80004d1c:	00048513          	mv	a0,s1
    80004d20:	00813483          	ld	s1,8(sp)
    80004d24:	02010113          	addi	sp,sp,32
    80004d28:	ffffe317          	auipc	t1,0xffffe
    80004d2c:	6d430067          	jr	1748(t1) # 800033fc <plic_complete>
    80004d30:	fffff097          	auipc	ra,0xfffff
    80004d34:	3e0080e7          	jalr	992(ra) # 80004110 <uartintr>
    80004d38:	fddff06f          	j	80004d14 <console_handler+0x74>
    80004d3c:	00000517          	auipc	a0,0x0
    80004d40:	5b450513          	addi	a0,a0,1460 # 800052f0 <digits+0x78>
    80004d44:	fffff097          	auipc	ra,0xfffff
    80004d48:	a68080e7          	jalr	-1432(ra) # 800037ac <panic>
	...
