# config-sync

`config-sync` is a lightweight command-line utility designed to manage, package, and synchronize configuration files between different systems. It allows you to track local configurations, bundle them into portable zip archives, and maintain synchronization with a remote repository.

## Installing

To install `config-sync`, clone the repository and use the provided installation script (or move the binary to your PATH).

```bash
git clone https://github.com/GhoulKingR/config-sync
cd config-sync
make install 
```

## Usage

The general syntax for this tool is:
```
Usage: config-sync [command] [flags]


Commands:
        init            Initialize config-sync on your system
        remote          Set a remote git repository for this project
        clone           Clone an existing config repository
        pull            Pull updates from remote repository
        push            Push recent updates to remote repository
        export          Export config files to zip file
        import          Import configs files from zip file
        status          Show the status of local setup
        help            Show this help text


Flags:
        --dry-run
        -v[v[v]]
        -f FILE, --file=FILE
        -u URL, --url=URL
        -n NAME, --name=NAME
```

### Initializing configs

Before managing your configurations, you must initialize the local workspace. This creates the necessary internal tracking files:
```bash
config-sync init
```

> You should also run the `init` command after updating the config file at _.config-sync/config.toml_

This setup prepares your environment to begin tracking local paths.

### Adding a config

To start tracking a specific configuration file or directory, Edit the _~/.config-sync/config.toml_ file, under `[targets]` with the new entry, following this syntax: `name = "/absolute/path/to/config"`. For example:

```bash
[targets]
nvim = "/Users/username/.config/nvim"
tmux = "/Users/username/.tmux.conf"
```

### Exporting to zip file

You can package all your configurations into a single  `.zip` archive. This is useful for manual backups or transferring settings to a machine without repository access.

```bash
config-sync export --file=backup.zip
```

### Importing from zip file

To restore or apply configurations from a previously exported archive, use the `import` command. This will unpack the files and place them back into their original registered paths.

```bash
config-sync import --file=backup.zip
```

### Initializing remote repository

For remote backups, you can link your local configurations to a Git repository. This allows you to push and pull changes rather than just using zip files.

```bash
config-sync remote --url=https://github.com/user/my-configs.git --name=origin
```

Once initialized, you can use `push` and `pull` commands to keep your configurations in sync with the repository.
```bash
# Push
config-sync push --name=origin

# pull
config-sync pull --name=origin
```


## Contributing

Contributions are welcome\! Please feel free to submit a Pull Request.

## License

This project is licensed under the [MIT License](LICENSE).
