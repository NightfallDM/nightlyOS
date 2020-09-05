//date: 2020/08/30


/*
 *Structrue that use in "mp.c" for MultiProcessor
 * */
#ifndef _NTFLDM_MP_H
#define _NTFLDM_MP_H

struct mp { // floating pointer
	uint8_t signature[4];  // "_MP_"
	void * physaddr; // phys addr of MP config table
	uint8_t length; 
	uint8_t specrev; // [14]
	uint8_t checksum; // all bytes must add up to 0
	uint8_t type; // MP system config type
};

struct mpconf { // configuration table header
	uint8_t signature[4]; // "PCMP"
	uint8_t length; // total table length
	uint8_t version; // [14]
	uint8_t checksum; // all bytes must add up to 0
	uint8_t product[20]; // product id
	uint32_t *oemtable; // OEM table pointer
	uint16_t oemlength; // OEM table length
	uint16_t entry; // entry count
	uint32_t *lapicaddr; // address of local APIC
	uint16_t xlength; // extended table length
	uint8_t xchecksum; // extended table checksum
	uint8_t reserved;
};

struct mpproc { // processor table entry
	uint8_t type; // entry type (0)
	uint8_t apicid; // local APIC id
	uint8_t version; // local APIC Version
	uint8_t flags; // CPU flags 0x02 mean the bootstrap processor
#define MPBOOT 0x02 // This proc is the bootstrap processor
	uint8_t signature[4]; // CPU signature
	uint32_t feature;
	uint8_t reserved[8];

};

struct mpioapic { // I/O APIC table entry
	uint8_t type; // entry type (2)
	uint8_t apicno; // I/O APIC id
	uint8_t version; // I/O APIC version
	uint8_t flags; // I/O APIC flags
	uint32_t *addr; // I/O APIC address

};

// Table entry types
#define MPPROC 0x00 // One per processor
#define MPBUS 0x01 // One per bus
#define MPIOAPIC 0x02 // One per I/O APIC
#define MPIOINTR 0x03 // One per bus interrupt source
#define MPLINTR 0x04 // One per system interrupt source

#endif // end of _NTFLDM_MP_H
