### PSEFM-C

PSEFM-C is a coordinate real time programming language which is based on c language.


### How to Install

#### Prepare

Simulation Environment: Ubuntu 12.04(above), 32 bit required
Simulator: qemu
RTOS: FreeRTOS
Architecture: STM32-P103

#### Reliable libraries

```
sudo apt-get install git zlib1g-dev libsdl1.2-dev automake*  autoconf* libtool libpixman-1-dev
```

#### Compiler

GNU Toolchain: Toolchain-2014_05.tar.bz2

```
mv Toolchain-2014_05.tar.bz2 /tmp
cd /
sudo tar jxvf /tmp/Toolchain-2014_05.tar.bz2
```

After Install, there will be a directory called arm-2014.05 under /usr/local/csl/
```
ls /usr/local/csl/
```

Then set the PATH:
```
export PATH=/usr/local/csl/arm-2014.05/bin:$PATH
```
(You can also write it to ~/.bashrc, then source ~/.bashrc)

#### Install Qemu
```
git clone git://github.com/beckus/qemu_stm32.git
cd qemu_stm32
git submodule update --init dtc || sudo apt-get install libfdt-dev
./configure --disable-werror --enable-debug \
    --target-list="arm-softmmu" \
    --extra-cflags=-DDEBUG_CLKTREE \
    --extra-cflags=-DDEBUG_STM32_RCC \
    --extra-cflags=-DDEBUG_STM32_UART \
    --extra-cflags=-DSTM32_UART_NO_BAUD_DELAY \
    --extra-cflags=-DSTM32_UART_ENABLE_OVERRUN \
    --disable-gtk
make -j4
```

#### install PSEFM
```
git clone git://github.com/wanborj/PSEFM-C.git
cd PSEFM-C
make qemu
```

The PSEFM example app in ./apps will run after that.


### How to use PSEFM-C

#### Programming API

##### Servant Create

```
ps_servant_create(servant_id, servant_type, subLET, num_of_src, src_array[], runnable);

servant_id   : The ID of a servant, and this is specified by programmer( must start from 0 )
servant_type : 0 for sensor; 1 for contoller; 2 for actuator
subLET       : The logical execution time duration of servant. It's specified by programmer according to the timing requirements
num_of_src   : The number of servants that communicate to this servant
src_array[]  : The array of pointer of source servants
runnable     : The run-time function this servant will be invoked
```
#### Task Create
```
ps_task_create(task_id, LET, servant_array[], num_of_servants)

task_id: The ID of a task, and this is specified by programmer( must start from 0 )
LET: The logical execution time of task. It's specified by programmer according to the timing requirements
servant_array[]: the array of pointer of servants in this task
num_of_servants: the number of servants in servant_array[]
```

#### Mode Create
```
ps_mode_create(mode_id, task_array[], num_of_tasks)

mode_id: the ID of a mode
task_array[]: the array of pointer of tasks in this mode
num_of_tasks: the number of tasks in task_array[]
```

#### Mode Switch Create
```
ps_mode_switch_create( bool (*switch_condition)(void ),  destination_mode)

switch_condition: The condition that mode switch happened
destination_mode: The target mode after mode switch
```

#### PSEFM start
```
system_start()
```



