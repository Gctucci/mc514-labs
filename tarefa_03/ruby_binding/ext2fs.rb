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
	
	
	
	# Structure of the super block
	class Ext2_super_block < FFI::Struct
		layout(
			:s_inodes_count,	:uint32,		# Inodes count 
			:s_blocks_count,	:uint32,		# Blocks count 
			:s_r_blocks_count,	:uint32,	# Reserved blocks count 
			:s_free_blocks_count,	:uint32,	# Free blocks count 
			:s_free_inodes_count,	:uint32,	# Free inodes count 
			:s_first_data_block,	:uint32,	# First Data Block 
			:s_log_block_size,	:uint32,	# Block size 
			:s_log_frag_size,	:int32,	# Fragment size 
			:s_blocks_per_group,	:uint32,	# # Blocks per group 
			:s_frags_per_group,	:uint32,	# # Fragments per group 
			:s_inodes_per_group,	:uint32,	# # Inodes per group 
			:s_mtime,	:uint32,		# Mount time 
			:s_wtime,	:uint32,		# Write time 
			:s_mnt_count,	:uint16,		# Mount count 
			:s_max_mnt_count,	:int16,	# Maximal mount count 
			:s_magic,	:uint16,		# Magic signature 
			:s_state,	:uint16,		# File system state 
			:s_errors,	:uint16,		# Behaviour when detecting errors 
			:s_minor_rev_level,	:uint16,	# minor revision level 
			:s_lastcheck,	:uint32,		# time of last check 
			:s_checkinterval,	:uint32,	# max. time between checks 
			:s_creator_os,	:uint32,		# OS 
			:s_rev_level,	:uint32,		# Revision level 
			:s_def_resuid,	:uint16,		# Default uid for reserved blocks 
			:s_def_resgid,	:uint16,		# Default gid for reserved blocks 
			
			# These fields are for EXT2_DYNAMIC_REV superblocks only.
			#
			# Note: the difference between the compatible feature set and
			# the incompatible feature set is that if there is a bit set
			# in the incompatible feature set that the kernel doesn't
			# know about, it should refuse to mount the filesystem.
			#
			# e2fsck's requirements are more strict; if it doesn't know
			# about a feature in either the compatible or incompatible
			# feature set, it must abort and not try to meddle with
			# things it doesn't understand...
			# 
			:s_first_ino,	:uint32,		# First non-reserved inode 
			:s_inode_size,	:uint16,		# size of inode structure 
			:s_block_group_nr,	:uint16,	# block group # of this superblock 
			:s_feature_compat,	:uint32,	# compatible feature set 
			:s_feature_incompat,	:uint32,	# incompatible feature set 
			:s_feature_ro_compat,	:uint32,	# readonly-compatible feature set 
			:s_uuid,	:uint8,	16,		# 128-bit uuid for volume 
			:s_volume_name,	:char,	16,	# volume name 
			:s_last_mounted,	:char,	64,	# directory where last mounted 
			:s_algorithm_usage_bitmap,	:uint32, # For compression 
			
			# Performance hints.  Directory preallocation should only
			# happen if the EXT2_FEATURE_COMPAT_DIR_PREALLOC flag is on.
			# 
			:s_prealloc_blocks,	:uint8,	# Nr of blocks to try to preallocate
			:s_prealloc_dir_blocks,	:uint8,	# Nr to preallocate for dirs 
			:s_reserved_gdt_blocks,	:uint16,	# Per group table for online growth
			
			# Journaling support valid if EXT2_FEATURE_COMPAT_HAS_JOURNAL set.
			# 
			:s_journal_uuid,	:uint8,	16,	# uuid of journal superblock 
			:s_journal_inum,	:uint32,		# inode number of journal file 
			:s_journal_dev,	:uint32,		# device number of journal file 
			:s_last_orphan,	:uint32,		# start of list of inodes to delete 
			:s_hash_seed,	:uint32,	4,		# HTREE hash seed 
			:s_def_hash_version,	:uint8,	# Default hash version to use 
			:s_jnl_backup_type,	:uint8, 	# Default type of journal backup 
			:s_desc_size,	:uint16,		# Group desc. size: INCOMPAT_64BIT 
			:s_default_mount_opts,	:uint32,
			:s_first_meta_bg,	:uint32,	# First metablock group 
			:s_mkfs_time,	:uint32,		# When the filesystem was created 
			:s_jnl_blocks,	:uint32,	17, 	# Backup of the journal inode 
			:s_blocks_count_hi,	:uint32,	# Blocks count high 32bits 
			:s_r_blocks_count_hi,	:uint32,	# Reserved blocks count high 32 bits
			:s_free_blocks_hi,	:uint32, 	# Free blocks count 
			:s_min_extra_isize,	:uint16,	# All inodes have at least # bytes 
			:s_want_extra_isize,	:uint16, 	# New inodes should reserve # bytes 
			:s_flags,	:uint32,		# Miscellaneous flags 
			:s_raid_stride,	:uint16,		# RAID stride 
			:s_mmp_interval,	:uint16,         # # seconds to wait in MMP checking 
			:s_mmp_block,	:uint64,            # Block for multi-mount protection 
			:s_raid_stripe_width,	:uint32,    # blocks on all data disks (N*stride)
			:s_log_groups_per_flex,	:uint8,	# FLEX_BG group size 
			:s_reserved_char_pad,	:uint8,
			:s_reserved_pad,	:uint16,		# Padding to next 32bits 
			:s_kbytes_written,	:uint64,	# nr of lifetime kilobytes written 
			:s_snapshot_inum,	:uint32,	# Inode number of active snapshot 
			:s_snapshot_id,	:uint32,		# sequential ID of active snapshot 
			:s_snapshot_r_blocks_count,	:uint64, # reserved blocks for active snapshot's future use 
			:s_snapshot_list,	:uint32,	# inode number of the head of the on-disk snapshot list 
		#define EXT4_S_ERR_START ext4_offsetof(struct ext2_super_block, s_error_count)
			:s_error_count,	:uint32,		# number of fs errors 
			:s_first_error_time,	:uint32,	# first time an error happened 
			:s_first_error_ino,	:uint32,	# inode involved in first error 
			:s_first_error_block,	:uint64,	# block involved of first error 
			:s_first_error_func,	:uint8,	32,	# function where the error happened 
			:s_first_error_line,	:uint32,	# line number where error happened 
			:s_last_error_time,	:uint32,	# most recent time of an error 
			:s_last_error_ino,	:uint32,	# inode involved in last error 
			:s_last_error_line,	:uint32,	# line number where error happened 
			:s_last_error_block,	:uint64,	# block involved of last error 
			:s_last_error_func,	:uint8,	32,	# function where the error happened 
		#define EXT4_S_ERR_END ext4_offsetof(struct ext2_super_block, s_error_count)
			:s_reserved,	:uint32,	128        # Padding to the end of the block 
		)
	end
	
	
	# Structure of a directory entry
	EXT2_NAME_LEN	= 255
	class Ext2_dir_entry  < FFI::Struct
		layout(
			:inode,	:uint32,            	# Inode number 
			:rec_len,	:uint16,        	# Directory entry length 
			:name_len,	:uint16,        	# Name length 
			:name,	:char,	EXT2_NAME_LEN	# File name 
		)
	end
	
	
	# The new version of the directory entry.  Since EXT2 structures are
	# stored in intel byte order, and the name_len field could never be
	# bigger than 255 chars, it's safe to reclaim the extra byte for the
	# file_type field.
	class Ext2_dir_entry_2  < FFI::Struct
		layout(
			:inode,	:uint32,			# Inode number 
			:rec_len,	:uint16,		# Directory entry length 
			:name_len,	:uint8, 		# Name length 
			:file_type,	:uint8,
			:name,	:char,	EXT2_NAME_LEN	# File name 
		)
	end
	
	
	# This structure will be used for multiple mount protection. It will be
	# written into the block number saved in the s_mmp_block field in the
	# superblock.
	EXT2_MMP_MAGIC  	= 0x004D4D50 # ASCII for MMP 
	EXT2_MMP_CLEAN  	= 0xFF4D4D50 # Value of mmp_seq for clean unmount 
	EXT2_MMP_FSCK_ON	= 0xE24D4D50 # Value of mmp_seq when being fscked 
	class Mmp_struct < FFI::Struct
		layout(
			:mmp_magic,	:uint32,
			:mmp_seq,	:uint32,
			:mmp_time,	:uint64,
			:mmp_nodename,	:char,	64,
			:mmp_bdevname,	:char,	32,
			:mmp_interval,	:uint16,
			:mmp_pad1,	:uint16,
			:mmp_pad2,	:uint32
		)
	end
	
	
	###
	#  ext2fs.h
	#  Contém as funções
	###
	
	# extern errcode_t ext2fs_open_inode_scan(ext2_filsys fs, int buffer_blocks,
	# ext2_inode_scan *ret_scan);
	attach_function :ext2fs_open_inode_scan, [Ext2_filsys, :int, Ext2_inode_scan], :pointer
end
