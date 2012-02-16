default:
	make -C src
	./update_image.sh

clean:
	make -C src clean

run: default
	./run_bochs.sh
	

