package fr.kissy.hellion.server.config;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.data.mongodb.core.MongoFactoryBean;
import org.springframework.data.mongodb.core.MongoTemplate;
import org.springframework.data.mongodb.repository.support.MongoRepositoryFactory;

@Configuration
public class DatabaseConfig {
    @Value("${database.host}")
    private String databaseHost;
    @Value("${database.name}")
    private String databaseName;

    @Bean
    public MongoFactoryBean mongoFactoryBean() {
        MongoFactoryBean mongoFactoryBean = new MongoFactoryBean();
        mongoFactoryBean.setHost(databaseHost);
        return mongoFactoryBean;
    }

    @Bean
    public MongoTemplate mongoTemplate() throws Exception {
        return new MongoTemplate(mongoFactoryBean().getObject(), databaseName);
    }

    @Bean
    public MongoRepositoryFactory mongoRepositoryFactory() throws Exception {
        return new MongoRepositoryFactory(mongoTemplate());
    }

}