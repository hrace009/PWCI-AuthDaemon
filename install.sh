#!/bin/bash

# Define Log File Name
LOGFILE="install_qt.log"

# Função para adicionar mensagens ao log
log() {
    echo "$(date '+%Y-%m-%d %H:%M:%S') - $1" >> $LOGFILE
}

# Inicializa o arquivo de log
echo "Início da instalação - $(date)" > $LOGFILE

# Atualiza os pacotes existentes e exibe o progresso no terminal e no log
log "Atualizando pacotes existentes..."
echo "Atualizando pacotes existentes..."
if ! sudo dnf update -y | tee -a $LOGFILE; then
    log "Erro ao atualizar pacotes."
    exit 1
fi

# Reinstala os pacotes gcc, gcc-c++ e glibc-devel
log "Reinstalando pacotes gcc, gcc-c++ e glibc-devel..."
echo "Reinstalando pacotes gcc, gcc-c++ e glibc-devel..."
if ! sudo dnf reinstall -y gcc gcc-c++ glibc-devel | tee -a $LOGFILE; then
    log "Erro ao reinstalar pacotes gcc, gcc-c++ e glibc-devel."
    exit 1
fi

# Instala o grupo de ferramentas de desenvolvimento e exibe o progresso no terminal e no log
log "Instalando Development Tools..."
echo "Instalando Development Tools..."
if ! sudo dnf groupinstall "Development Tools" -y | tee -a $LOGFILE; then
    log "Erro ao instalar Development Tools."
    exit 1
fi

# Instala os pacotes necessários e exibe o progresso no terminal e no log
log "Instalando pacotes qt5..."
echo "Instalando pacotes qt5..."
if ! sudo dnf install -y qt5-qtbase-devel qt5-qtx11extras qt5-qttools | tee -a $LOGFILE; then
    log "Erro ao instalar pacotes qt5."
    exit 1
fi

# Remove pacotes desnecessários e exibe o progresso no terminal e no log
log "Removendo pacotes desnecessários..."
echo "Removendo pacotes desnecessários..."
if ! sudo dnf autoremove -y | tee -a $LOGFILE; then
    log "Erro ao remover pacotes desnecessários."
    exit 1
fi

# Instala bibliotecas adicionais e exibe o progresso no terminal e no log
log "Instalando bibliotecas adicionais..."
echo "Instalando bibliotecas adicionais..."
if ! sudo dnf install -y qt5-qttools-devel | tee -a $LOGFILE; then
    log "Erro ao instalar bibliotecas adicionais."
    exit 1
fi

# Procura por pacotes relacionados ao qt5 e exibe o progresso no terminal e no log
log "Procurando pacotes relacionados ao qt5..."
echo "Procurando pacotes relacionados ao qt5..."
if ! sudo dnf search qt5 | tee -a $LOGFILE; then
    log "Erro ao procurar pacotes relacionados ao qt5."
    exit 1
fi

# Finaliza o arquivo de log
log "Instalação concluída - $(date)"
echo "Instalação concluída - $(date)" >> $LOGFILE
