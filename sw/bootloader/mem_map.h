#ifndef __MEM_MAP_H__
#define __MEM_MAP_H__

//-----------------------------------------------------------------
// Defines:
//-----------------------------------------------------------------
#define INT_BASE			0x00000000
#define INT_APP_BASE        0x00002000
#define EXT_BASE			0x10000000
#define IO_BASE				0x20000000
#define EXT_IO_BASE			0x30000000
#define EXT_DP_BASE			0x40000000

#define REGION_INTERNAL		0
#define REGION_EXTERNAL		1
#define REGION_CORE_IO		2
#define REGION_EXT_IO		3
#define REGION_DPRAM		4

#define REGION_OFFSET		28
#define REGION_MASK			0x7

#define IMEM				__attribute__ ((section (".fastmem")))

//-----------------------------------------------------------------
// Macros:
//-----------------------------------------------------------------
#define REG8				(volatile unsigned char*)
#define REG16				(volatile unsigned short*)
#define REG32				(volatile unsigned int*)

//-----------------------------------------------------------------
// I/O:
//-----------------------------------------------------------------

// General
#define CORE_ID				(*(REG32 (IO_BASE + 0x0)))

// Basic Peripherals
#define UART_USR			(*(REG32 (IO_BASE + 0x4)))
#define UART_UDR			(*(REG32 (IO_BASE + 0x8)))
#define TIMER_VAL			(*(REG32 (IO_BASE + 0x10)))
#define IRQ_MASK_SET		(*(REG32 (IO_BASE + 0x14)))
#define IRQ_MASK_STATUS		(*(REG32 (IO_BASE + 0x14)))
#define IRQ_MASK_CLR		(*(REG32 (IO_BASE + 0x18)))
#define IRQ_STATUS			(*(REG32 (IO_BASE + 0x1C)))
	#define IRQ_SYSTICK			(0)
	#define IRQ_UART_RX_AVAIL   (1)
	#define IRQ_SW			    (2)
	#define IRQ_GPIO1			(3)	
	#define IRQ_SPI_UEXT		(5)
	#define IRQ_PIT				(6)
	#define EXT_INT_OFFSET		(8)
	#define IRQ_SPI_SD			(EXT_INT_OFFSET + 0)
	#define IRQ_IR				(EXT_INT_OFFSET + 1)
	#define IRQ_AUDIO_FIFO		(EXT_INT_OFFSET + 2)
	#define IRQ_UEXT2_5			(EXT_INT_OFFSET + 3)
	#define IRQ_DMA				(EXT_INT_OFFSET + 4)

#define WATCHDOG_CTRL		(*(REG32 (IO_BASE + 0x20)))
	#define WATCHDOG_EXPIRED	(16)

#define SYS_CLK_COUNT		(*(REG32 (IO_BASE + 0x60)))

// SPI Configuration PROM
#define SPI_PROM_CTRL		(*(REG32 (IO_BASE + 0x70)))
	#define SPI_PROM_CS			(1 << 0)
#define SPI_PROM_STAT		(*(REG32 (IO_BASE + 0x70)))
	#define SPI_PROM_BUSY		(1 << 0)
#define SPI_PROM_DATA		(*(REG32 (IO_BASE + 0x74)))

// Extended Peripherals

// Simple SPI Interface
#define SPI_EXT_CTRL		(*(REG32 (EXT_IO_BASE + 0x10)))
	#define SPI_EXT_CS			(1 << 0)
#define SPI_EXT_STAT		(*(REG32 (EXT_IO_BASE + 0x10)))
	#define SPI_EXT_BUSY		(1 << 0)
#define SPI_EXT_DATA		(*(REG32 (EXT_IO_BASE + 0x14)))

// Secondary Core Control
#define CPU2_CTRL			(*(REG32 (EXT_IO_BASE + 0x20)))
	#define CPU2_CTRL_RESET		(1 << 0)
	#define CPU2_INTERRUPT		(1 << 1)
#define CPU2_STAT			(*(REG32 (EXT_IO_BASE + 0x20)))
	#define CPU2_STAT_ENABLED	(1 << 0)
	#define CPU2_STAT_FAULT		(1 << 1)

// UEXTx
#define UEXT_GPIO_OUT(x)	(*(REG32 (EXT_IO_BASE + 0x20 + (16 * x))))
#define UEXT_GPIO_IN(x)		(*(REG32 (EXT_IO_BASE + 0x24 + (16 * x))))
#define UEXT_GPIO_DDR(x)	(*(REG32 (EXT_IO_BASE + 0x28 + (16 * x))))

// UEXT 1
#define GPIO_OUT			(*(REG32 (EXT_IO_BASE + 0x20)))
#define GPIO_IN				(*(REG32 (EXT_IO_BASE + 0x24)))
#define GPIO_DDR			(*(REG32 (EXT_IO_BASE + 0x28)))

// UEXT 2 / SD
#define UEXT2_GPIO_OUT		(*(REG32 (EXT_IO_BASE + 0x30)))
#define UEXT2_GPIO_IN		(*(REG32 (EXT_IO_BASE + 0x34)))
#define UEXT2_GPIO_DDR		(*(REG32 (EXT_IO_BASE + 0x38)))
	#define UEXT2_SD_CS			(8)

// SPI DMA
#define SPI_DMA_CTRL(x)		(*(REG32 (EXT_IO_BASE + 0x40 + (16 * x))))
#define SPI_DMA_STAT(x)		(*(REG32 (EXT_IO_BASE + 0x40 + (16 * x))))
#define SPI_DMA_ADDR(x)		(*(REG32 (EXT_IO_BASE + 0x44 + (16 * x))))
#define SPI_DMA_DATA(x)		(*(REG32 (EXT_IO_BASE + 0x48 + (16 * x))))

// Multiplier
#define MULT_OPERAND_A		(*(REG32 (EXT_IO_BASE + 0x60)))
#define MULT_OPERAND_B		(*(REG32 (EXT_IO_BASE + 0x64)))
#define MULT_RESULT_HI		(*(REG32 (EXT_IO_BASE + 0x60)))
#define MULT_RESULT_LO		(*(REG32 (EXT_IO_BASE + 0x64)))

// Audio FIFO
#define FIFO_WRITE			(*(REG32 (EXT_IO_BASE + 0x70)))
#define FIFO_UNDERUNS		(*(REG32 (EXT_IO_BASE + 0x70)))
#define FIFO_STATUS			(*(REG32 (EXT_IO_BASE + 0x74)))
	#define FIFO_STAT_EMPTY		(1 << 0)
	#define FIFO_STAT_FULL		(1 << 1)
	#define FIFO_STAT_LOW		(1 << 2)
	#define FIFO_STAT_CNT_SHIFT		8
#define FIFO_IRQ_THRESH		(*(REG32 (EXT_IO_BASE + 0x74)))

// IR Decoder
#define IR_CTRL				(*(REG32 (EXT_IO_BASE + 0x80)))
#define IR_STAT				(*(REG32 (EXT_IO_BASE + 0x80)))
	#define IR_STAT_IR_IN		(1 << 0)
	#define IR_STAT_BUSY		(1 << 1)
#define IR_DATA				(*(REG32 (EXT_IO_BASE + 0x84)))

// DMA Controller
#define DMA_CTRL			(*(REG32 (EXT_IO_BASE + 0xA0)))
#define DMA_STAT			(*(REG32 (EXT_IO_BASE + 0xA0)))
	#define DMA_STAT_RDY		(1 << 0)
	#define DMA_STAT_BUSY		(1 << 1)
#define DMA_SRC_ADDR		(*(REG32 (EXT_IO_BASE + 0xA4)))
#define DMA_DST_ADDR		(*(REG32 (EXT_IO_BASE + 0xA8)))
#define DMA_COUNT			(*(REG32 (EXT_IO_BASE + 0xAC)))

#define RAM_TIMING			(*(REG32 (EXT_IO_BASE + 0xE0)))

#endif 
