#!/bin/sh

# Terminal customizations

## Install oh-my-posh terminal

    ### create a bin dir in case in doesn't exists
if ! [ -d "$HOME/bin"]; then
    mkdir ~/bin
    echo "\nexport PATH=$HOME/bin:$PATH\n" >> $HOME/.bashrc
    echo "\nexport PATH=$HOME/bin:$PATH\n" >> $HOME/.zshrc
fi

export PATH=$HOME/bin:$PATH
curl -s https://ohmyposh.dev/install.sh | bash -s -- -d $HOME/bin

## Persist and Use one of the themes
echo  "eval \"\$(oh-my-posh init bash --config $HOME/.cache/oh-my-posh/themes/stelbent-compact.minimal.omp.json)\"\n" >> $HOME/.bashrc
echo  "eval \"\$(oh-my-posh init zsh  --config $HOME/.cache/oh-my-posh/themes/stelbent-compact.minimal.omp.json)\"\n" >> $HOME/.zshrc
