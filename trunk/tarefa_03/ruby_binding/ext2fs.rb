# ext2fs.rb

require 'ffi'

module Ext2fs
	extend FFI::Library
	ffi_lib "/usr/lib/libext2fs.so"
	
	###
	#  ext2_fs.h
	#  Contém as estruturas do inode e do superbloco
	###
# Constants relative to the data blocks
EXT2_NDIR_BLOCKS	= 12
EXT2_IND_BLOCK  	= EXT2_NDIR_BLOCKS
EXT2_DIND_BLOCK 	= (EXT2_IND_BLOCK + 1)
EXT2_TIND_BLOCK 	= (EXT2_DIND_BLOCK + 1)
EXT2_N_BLOCKS   	= (EXT2_TIND_BLOCK + 1)
	
	# Structure of an inode on the disk
#	union {
#		struct {
#			__u32	l_i_version; # was l_i_reserved1 
#		} linux1;
#		struct {
#			__u32  h_i_translator;
#		} hurd1;
#	} osd1;				# OS dependent 1 
	class Osd1 < FFI::Union
		class Linux1 < FFI::Union
			layout(:l_i_version, :pointer)
		end
		class Hurd1 < FFI::Union
			layout(:h_i_translator, :pointer)
		end
		layout(
			:linux1,	Linux1,
			:hurd1, 	Hurd1
		)
	end 	# OS dependent 1 
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
	class Osd2 < FFI::Union
		class Linux2 < FFI::Union
			layout(
				:l_i_blocks_hi, 	:pointer,
				:l_i_file_acl_high,	:pointer,
				:l_i_uid_high,  	:pointer,  	# these 2 fields    
				:l_i_gid_high,  	:pointer,  	# were reserved2[0] 
				:l_i_reserved2, 	:pointer
			)
		end
		class Hurd2 < FFI::Union
			layout(
				:h_i_frag,  	:pointer,  	# Fragment number 
				:h_i_fsize, 	:pointer, 	# Fragment size 
				:h_i_mode_high,	:pointer,
				:h_i_uid_high,	:pointer,
				:h_i_gid_high,	:pointer,
				:h_i_author,	:pointer
			)
		end
		layout(
			:linux2,	Linux2,
			:hurd2, 	Hurd2
		)
	end 	# OS dependent 2 
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
			:osd1,      	Osd1,   	# OS dependent 1
			:i_block,   	:pointer,	EXT2_N_BLOCKS, 	# Pointers to blocks 
			:i_generation,	:pointer,	# File version (for NFS) 
			:i_file_acl,	:pointer,	# File ACL 
			:i_dir_acl, 	:pointer,	# Directory ACL 
			:i_faddr,   	:pointer,	# Fragment address 
			:osd2,      	Osd2    	# OS dependent 2
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
