# ext2fs.rb

require 'ffi'

module Ext2fs
	extend FFI::Library
	ffi_lib "/usr/lib/libext2fs.so"
	
	###
	#  ext2_fs.h
	#  Contém as estruturas do inode e do superbloco
	###
	
	# Structure of an inode on the disk
	class Ext2_inode < FFI::Struct
		layout(
			:i_mode,    	:pointer,	# File mode 
			:i_uid,     	:pointer,	# Low 16 bits of Owner, Uid 
			:i_size,    	:pointer,	# Size in bytes 
			:i_atime,   	:pointer,	# Access time 
			:i_ctime,   	:pointer,	# Inode change time 
			:i_mtime,   	:pointer,	# Modification time 
			:i_dtime,   	:pointer,	# Deletion Time 
			:i_gid,     	:pointer,	# Low 16 bits of Group Id 
			:i_links_count,	:pointer,	# Links count 
			:i_blocks,  	:pointer,	# Blocks count 
			:i_flags,   	:pointer,	# File flags 
#	union {
#		struct {
#			__u32	l_i_version; # was l_i_reserved1 
#		} linux1;
#		struct {
#			__u32  h_i_translator;
#		} hurd1;
#	} osd1;				# OS dependent 1 
#	__u32	i_block[EXT2_N_BLOCKS];# Pointers to blocks 
#	__u32	i_generation;	# File version (for NFS) 
#	__u32	i_file_acl;	# File ACL 
#	__u32	i_dir_acl;	# Directory ACL 
#	__u32	i_faddr;	# Fragment address 
#	union {
#		struct {
#			__u16	l_i_blocks_hi;
#			__u16	l_i_file_acl_high;
#			__u16	l_i_uid_high;	# these 2 fields    
#			__u16	l_i_gid_high;	# were reserved2[0] 
#			__u32	l_i_reserved2;
#		} linux2;
#		struct {
#			__u8	h_i_frag;	# Fragment number 
#			__u8	h_i_fsize;	# Fragment size 
#			__u16	h_i_mode_high;
#			__u16	h_i_uid_high;
#			__u16	h_i_gid_high;
#			__u32	h_i_author;
#		} hurd2;
#	} osd2;				# OS dependent 2 
		)
	end	
	
	###
	#  ext2fs.h
	#  Contém as funções
	###
	
	# extern errcode_t ext2fs_open_inode_scan(ext2_filsys fs, int buffer_blocks,
	# ext2_inode_scan *ret_scan);
	attach_function :ext2fs_open_inode_scan, [:pointer, :int, :pointer], :pointer
	
	
end
