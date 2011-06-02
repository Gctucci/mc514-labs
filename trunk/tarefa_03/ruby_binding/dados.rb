require 'ffi'
require 'ext2fs'

if ARGV.count < 2
	puts "Uso: ruby dados.rb [opção] [arquivo]"
	puts "\topção: Partição ou arquivo ext a ser examinado"
	puts "\t\t1: Fragmentação de blocos alocados em no. de segmentos"
	puts "\t\t2: Tamanhos de arquivos em blocos de alocação"
	puts "\tarquivo: Partição ou arquivo ext a ser examinado"
else
	if ARGV[0].to_i == 1
		puts "Histograma de fragmentação de blocos alocados em no. de segmentos"
		node = Ext2fs::Ext2_inode.new
	elsif ARGV[0].to_i == 2
		puts "Histograma de tamanhos de arquivos em blocos de alocação"
	else
		puts "Opção inválida"
	end
	puts 10
	puts 10
	puts 10
	puts 10
	puts 10
	puts 10
	puts 10
	puts 10
	puts 10
	puts 10
	puts 10
	puts 10
	puts 10
	puts 10
	puts 10
	puts 10
	puts 10
	puts 10
	puts 10
	puts 10
	puts 10
end
