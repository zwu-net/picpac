.PHONY:	help docker run

help:
	@echo "Usage:"
	@echo "  make docker # build docker"
	@echo "  make run    # run docker with current directory mounted at /picpac"

docker:
	nvidia-docker build -t aaalgo/tensorflow-picpac .

run:
	nvidia-docker run -it -v $(PWD):/root/picpac aaalgo/tensorflow-picpac /bin/bash
