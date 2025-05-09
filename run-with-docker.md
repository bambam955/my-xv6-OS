# Running XV6 in Docker

Initially, I was using a Lubuntu virtual machine to run this XV6 operating system, but I was having some issues with its feature set and it was also really slow. So, I decided to Dockerize the process instead.

Here are the steps for Windows users. I don't have a Mac to test with but I think the steps are comparable.

## Install Docker Desktop

1. Go to https://docs.docker.com/desktop/setup/install/ and download the Docker Installer.
  - The installers for Mac and Linux are easily available in the different install pages, listed in the sidebar on the left.
2. Run the installer application to install Docker Desktop.
3. Open a Command Prompt terminal (not BASH, it won't work right) and run the following:

    ```bash
    docker --version
    docker pull hello-world
    docker run hello-world
    ```

4. If you don't get any errors, then your installation of docker was successful.

## Set Up VS Code Dev Containers

1. If you don't already have VS Code, install it here: https://code.visualstudio.com/download
2. Install the "Dev Containers" extension from Microsoft: https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers. You can search "Dev Containers" in the extensions library in VS Code and it will come up.

## Run XV6 in a Container

1. Open this repository in VS Code.
2. Open the command prompt and run `Dev Containers: Reopen in Container`.
3. If it succeeds, then you will now be editing the repo files in a new Docker container!
4. Now, open a Command Prompt again.
5. Run the following command to get inside the container:

    ```bash
    docker exec -it xv6-dev-container bash
    ```

6. Now you will be located in the root of the repository. You can run `make && make qemu-nox` to run XV6.
7. Run `Remote: Close Remote Connection` in VS Code to stop the container.

## Push and Pull Git Changes

You can open your local copy of the repo in Git BASH to make commits, pull changes, and everything else.

All changes that you make inside the container will be automatically synced to your local copy of the repo and vice versa, so you can change files in VS Code, run the new code inside the container, and commit the changes using your local Git config and login.

You can do some Git operations directly in the container, such as `git status` and `git pull`; the only thing you can't do is `git push`, because your Git credentials aren't set up automatically in the container.