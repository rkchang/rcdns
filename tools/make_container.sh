#!/bin/sh
# Create a development container
docker build --tag rcdnsdev .

# Mount gitconfig for email and name 
# Mount current directory
# Mount ssh key
# Use fish as shell
docker create \
    --tty \
    --interactive \
    --name rcdnsdev \
    --volume ~/.gitconfig:/etc/gitconfig \
    --volume "$(pwd)":/rcdnsdev \
    --volume ~/.ssh/github:/root/.ssh/id_rsa \
    rcdnsdev \
    fish 
