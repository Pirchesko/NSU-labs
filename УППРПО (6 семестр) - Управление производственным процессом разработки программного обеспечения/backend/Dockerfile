FROM maven:3.8.5-jdk-11-slim

WORKDIR /usr/src/app
COPY . .
RUN mvn clean package spring-boot:repackage

EXPOSE 8181
CMD ["java", "-jar", "/usr/src/app/target/calc-back-1.0-SNAPSHOT.jar"]
