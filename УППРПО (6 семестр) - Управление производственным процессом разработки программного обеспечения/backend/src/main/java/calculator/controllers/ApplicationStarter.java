package calculator.controllers;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.EnableAutoConfiguration;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.autoconfigure.security.servlet.SecurityAutoConfiguration;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;


@Configuration
@EnableAutoConfiguration(exclude = { SecurityAutoConfiguration.class})
@ComponentScan({
        "calculator.model",
        "calculator.configs",
        "calculator.controllers",
        "calculator.algorithms",
        "calculator.algorithms.implementations"})
public class ApplicationStarter {
    public static void main(String [] args){

        SpringApplication.run(ApplicationStarter.class);
    }
}
