# Устанавливаем базовый образ
FROM ubuntu:latest

ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Обновление и установка зависимостей
RUN apt-get update
RUN apt-get install -y qt6-base-dev
RUN apt-get install -y build-essential
RUN apt-get install -y qtchooser 
RUN apt-get install -y qt6-base-dev-tools 
RUN apt-get install -y qmake6

# Копируем файлы проекта в контейнер
WORKDIR /Server1
COPY *.cpp /Server1
COPY *.h /Server1
COPY *.pro  /Server1
COPY *.db /Server1

# Сборка проекта
RUN qmake6 /Server1/serv.pro
RUN make

# Запускаем
#WORKDIR /test # Устанавливаем WORKDIR перед запуском
ENTRYPOINT ["./serv"] 
