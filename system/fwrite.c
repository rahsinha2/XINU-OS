#include <fs.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <thread.h>

int fwrite(int fd, void *buf, int nbytes)
{
  struct thrent *thrptr;
  int i;
  int seek;
  int block_no;
  int remaining;
  int data_block_no;
  int data_block_offset;
  int no_bytes_written;
  int byes_to_write;
  int start_poistion;
  int incore_inode_index;
  int filetablentry;

  
  if (nbytes == 0)
  {
      printf("\nInvalid number of bytes to write.");
      return 0;
  }
  
  if (buf == NULL)
  {
      printf("\nInvalid buffer pointer ");
      return 0;
  }
  
  thrptr = &thrtab[thrcurrent];
  if (thrptr->fdesc[fd] == UNUSED)
  {
      printf("\nInvalid File Descriptor %d.", fd);
      return 0;
  }
  
  filetablentry = thrptr->fdesc[fd];
  if (openfiletable[filetablentry].state == UNUSED)
  {
      printf("\nOpen file table entry not found.");
      return 0;
  }
  if (openfiletable[filetablentry].mode == O_RDONLY)
  {
      printf("\nFile not open for write/append.");
      return 0;
  }
  
  seek = openfiletable[filetablentry].fptr;
  incore_inode_index = openfiletable[filetablentry].in_core_inode_num;
  if(core_inode[incore_inode_index].id == UNUSED)
  {
      printf("\nCore Inode table entry not found");
      return 0;
  }
  
  //logic to write partially filled block
  data_block_no = seek / MDEV_BLOCK_SIZE;
  data_block_offset = seek % MDEV_BLOCK_SIZE;
  no_bytes_written = 0;
  remaining = nbytes;
  
  
  while( no_bytes_written < nbytes )
  {
      byes_to_write = ( remaining < (MDEV_BLOCK_SIZE - data_block_offset))? remaining : (MDEV_BLOCK_SIZE - data_block_offset);
      start_poistion  =  data_block_offset;
      
      block_no = block_no_from_datablock_no( incore_inode_index,  data_block_no);
      
      if(block_no == UNUSED)
      {
          block_no = allocate_block(incore_inode_index,  data_block_no);
      }
      
      if(block_no == SYSERR)
      {
          printf("\nInvalid Data block number");
          break;   
      }
      
      if (block_offset_write( 0, block_no, start_poistion, buf+no_bytes_written, byes_to_write) == SYSERR)
      {
          printf("\nError while writing data block");
          break;
      }
      
      no_bytes_written += byes_to_write;
      remaining -= byes_to_write;
      data_block_offset = 0;
      data_block_no++; 
  }
  
   // update file size and seek pointer.
    core_inode[incore_inode_index].size +=  no_bytes_written;
      openfiletable[filetablentry].fptr += no_bytes_written ;
  
  return no_bytes_written;
}


