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
	class Ext2_inode < FFI::Struct
		# OS dependent 1
		class Osd1 < FFI::Union
			class Linux1 < FFI::Union
				layout(:l_i_version, :uint32)
			end
			class Hurd1 < FFI::Union
				layout(:h_i_translator, :uint32)
			end
			layout(
				:linux1,	Linux1,
				:hurd1, 	Hurd1
			)
		end
		# OS dependent 2
		class Osd2 < FFI::Union
			class Linux2 < FFI::Union
				layout(
					:l_i_blocks_hi, 	:uint16,
					:l_i_file_acl_high,	:uint16,
					:l_i_uid_high,  	:uint16,  	# these 2 fields
					:l_i_gid_high,  	:uint16,  	# were reserved2[0]
					:l_i_reserved2, 	:uint32
				)
			end
			class Hurd2 < FFI::Union
				layout(
					:h_i_frag,  	:uint8,  	# Fragment number
					:h_i_fsize, 	:uint8, 	# Fragment size
					:h_i_mode_high,	:uint16,
					:h_i_uid_high,	:uint16,
					:h_i_gid_high,	:uint16,
					:h_i_author,	:uint32
				)
			end
			layout(
				:linux2,	Linux2,
				:hurd2, 	Hurd2
			)
		end
		layout(
			:i_mode,    	:uint16,	# File mode
			:i_uid,     	:uint16,	# Low 16 bits of Owner, Uid
			:i_size,    	:uint32,	# Size in bytes
			:i_atime,   	:uint32,	# Access time
			:i_ctime,   	:uint32,	# Inode change time
			:i_mtime,   	:uint32,	# Modification time
			:i_dtime,   	:uint32,	# Deletion Time
			:i_gid,     	:uint16,	# Low 16 bits of Group Id
			:i_links_count,	:uint16,	# Links count
			:i_blocks,  	:uint32,	# Blocks count
			:i_flags,   	:uint32,	# File flags
			:osd1,      	Osd1,   	# OS dependent 1
			:i_block,   	:uint32,	EXT2_N_BLOCKS, 	# uint32s to blocks
			:i_generation,	:uint32,	# File version (for NFS)
			:i_file_acl,	:uint32,	# File ACL
			:i_dir_acl, 	:uint32,	# Directory ACL
			:i_faddr,   	:uint32,	# Fragment address
			:osd2,      	Osd2    	# OS dependent 2
		)
	end	
	
	# Permanent part of an large inode on the disk
	class Ext2_inode_large < FFI::Struct
		# OS dependent 1
		class Osd1 < FFI::Union
			class Linux1 < FFI::Union
				layout(:i_reserved1 , :uint32)
			end
			class Hurd1 < FFI::Union
				layout(:i_translator, :uint32)
			end
			layout(
				:linux1,	Linux1,
				:hurd1, 	Hurd1
			)
		end
		# OS dependent 2
		class Osd2 < FFI::Union
			class Linux2 < FFI::Union
				layout(
					:i_blocks_hi,   	:uint16,
					:i_file_acl_high,	:uint16,
					:i_uid_high,    	:uint16,	# these 2 fields
					:i_gid_high,    	:uint16,	# were reserved2[0]
					:i_reserved2,   	:uint32
				)
			end
			class Hurd2 < FFI::Union
				layout(
					:i_frag,    	:uint8,	# Fragment number
					:i_fsize,   	:uint8,	# Fragment size
					:i_mode_high,	:uint16,
					:i_uid_high,	:uint16,
					:i_gid_high,	:uint16,
					:i_author,  	:uint32
				)
			end
			layout(
				:linux2,	Linux2,
				:hurd2, 	Hurd2
			)
		end
		layout(
			:i_mode,        	:uint16,	# File mode
			:i_uid,         	:uint16,	# Low 16 bits of Owner Uid
			:i_size,        	:uint32,	# Size in bytes
			:i_atime,       	:uint32,	# Access time
			:i_ctime,       	:uint32,	# Inode Change time
			:i_mtime,       	:uint32,	# Modification time
			:i_dtime,       	:uint32,	# Deletion Time
			:i_gid,         	:uint16,	# Low 16 bits of Group Id
			:i_links_count, 	:uint16,	# Links count
			:i_blocks,      	:uint32,	# Blocks count
			:i_flags,       	:uint32,	# File flags
			:osd1,          	Osd1,   	# OS dependent 1
			:i_block,       	:uint32,	EXT2_N_BLOCKS, # uint32s to blocks
			:i_generation,  	:uint32,	# File version (for NFS)
			:i_file_acl,    	:uint32,	# File ACL
			:i_dir_acl,     	:uint32,	# Directory ACL
			:i_faddr,       	:uint32,	# Fragment address
			:osd2,          	Osd2,   	# OS dependent 2
			:i_extra_isize, 	:uint16,
			:i_pad1,        	:uint16,
			:i_ctime_extra, 	:uint32,	# extra Change time (nsec << 2 | epoch)
			:i_mtime_extra, 	:uint32,	# extra Modification time (nsec << 2 | epoch)
			:i_atime_extra, 	:uint32,	# extra Access time (nsec << 2 | epoch)
			:i_crtime,      	:uint32,	# File creation time
			:i_crtime_extra,	:uint32,	# extra File creation time (nsec << 2 | epoch)
			:i_version_hi,  	:uint32		# high 32 bits for 64-bit version
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
