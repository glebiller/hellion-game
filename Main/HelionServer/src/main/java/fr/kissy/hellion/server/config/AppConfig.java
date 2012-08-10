package fr.kissy.hellion.server.config;

import fr.kissy.hellion.server.realm.ShiroBaseRealm;
import org.apache.shiro.SecurityUtils;
import org.apache.shiro.mgt.DefaultSecurityManager;
import org.apache.shiro.mgt.SecurityManager;
import org.apache.shiro.realm.Realm;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.ImportResource;
import org.springframework.data.mongodb.core.MongoFactoryBean;
import org.springframework.data.mongodb.core.MongoTemplate;

@Configuration
@ImportResource("classpath:/fr/kissy/hellion/server/properties-config.xml")
public class AppConfig {

    private @Value("${database.host}") String databaseHost;
    private @Value("${database.name}") String databaseName;

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
    public SecurityManager securityManager() {
        DefaultSecurityManager securityManager = new DefaultSecurityManager(realm());
        SecurityUtils.setSecurityManager(securityManager);
        return securityManager;
    }

    @Bean
    public Realm realm() {
        return new ShiroBaseRealm();
    }

}