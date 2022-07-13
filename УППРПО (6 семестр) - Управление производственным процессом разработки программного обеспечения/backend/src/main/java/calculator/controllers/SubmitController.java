package calculator.controllers;

import calculator.algorithms.Algorithm;
import calculator.algorithms.Validator;
import calculator.jsondecs.Request;
import calculator.jsondecs.Response;
import calculator.model.Matrix;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationContext;
import org.springframework.http.MediaType;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;


@CrossOrigin(origins={"http://localhost:3000", "http://62.84.120.70:3000", "chrome-extension://eelcnbccaccipfolokglfhhmapdchbfg"}, allowedHeaders = "*")
@RestController
public class SubmitController {
    @Autowired
    private ApplicationContext appContext;

    private Algorithm.Input reqToInput(Request request, Validator.WhatToValidate whatToValidate){
        Algorithm.Input input = new Algorithm.Input();
        input.q = request.getQ();
        if (whatToValidate.n) input.n = request.getN();
        if (whatToValidate.u){
            input.u = new int[request.getU().length];
            for (int i = 0; i < request.getU().length; i++) {
                input.u[i] = (request.getU()[i] == null) ? 0 : request.getU()[i];
            }
        }
        if (whatToValidate.g){
            input.g = new int[request.getG().length];
            for (int i = 0; i < request.getG().length; i++) {
                input.g[i] = (request.getG()[i] == null) ? 0 : request.getG()[i];
            }
        }
        if (whatToValidate.matrix){
            input.matrix = new Matrix(request.getMatrix(), request.getQ());
        }
        if (whatToValidate.d){
            input.d = request.getD();
        }
        if (whatToValidate.k){
            input.k = request.getK();
        }
        return input;
    }

    @PostMapping(value="/", produces = MediaType.APPLICATION_JSON_VALUE)
    public Response allSubmit(@RequestBody Request received){
        ClassLoader classloader = Thread.currentThread().getContextClassLoader();
        InputStream is = classloader.getResourceAsStream("actions.config");
        Properties prop = new Properties();
        try {
            prop.load(is);
        } catch (IOException exception) {
            System.out.println("LOGS1");
            return new Response(false, "Внутренняя ошибка, сообщите разработчикам", null, null);
        }
        if (received.getAction() < 0 || received.getAction() > 11)
            return new Response(false, "Ошибка: функция еще не реализована", null, null);

        Algorithm object = (Algorithm)appContext.getBean(prop.getProperty(String.valueOf(received.getAction())));

        Validator.ValidateResult validateResult = Validator.validate(received, object.whatToValidate());
        if (!validateResult.valid){
            return new Response(false, "Ошибка: " + validateResult.error, null, null);
        }

        var result = object.calculate(reqToInput(received, object.whatToValidate()));
        if (result.isStatus())
            return new Response(true, null, result.getAnswer(), null);
        else
            return new Response(false, "Ошибка: " + result.getAnswer0(), null, null);
    }

}
