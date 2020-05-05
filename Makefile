default:
	@echo "Использовать make sed или make par.\nДля запуска последовательной или параллельной версии."

seq:
	@echo "Последовательная программа:\n"
	g++ -o main seq_alg.cpp
	./main


par:
	@echo "Параллельная программа"
	mpicxx -o main par_alg.cpp
	mpirun -n 1 ./main