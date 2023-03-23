all:flock lockf flock_db lockf_db

flock:flock.c
	gcc -g flock.c -o flock

lockf:lockf.c
	gcc -g lockf.c -o lockf

flock_db:
	echo -n 3500 > flock.db

lockf_db:
	echo -n 3500 > lockf.db

clean:
	rm flock lockf flock.db lockf.db