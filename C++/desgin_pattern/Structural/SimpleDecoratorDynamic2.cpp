#include <memory>
#include <iostream>
#include <string>

struct WebPage
{
    virtual void display()=0;
    virtual ~WebPage() = default;
};

struct BasicWebPage : WebPage
{
    std::string html;
    void display() override
    {
        std::cout << "Basic WEB page" << std::endl;
    }
};

struct WebPageDecorator : WebPage
{
    WebPageDecorator(std::unique_ptr<WebPage> webPage): _webPage(std::move(webPage))
    {
    }
    void display() override
    {
        _webPage->display();
    }
private:
    std::unique_ptr<WebPage> _webPage;
};

struct AuthenticatedWebPage : WebPageDecorator
{
    AuthenticatedWebPage(std::unique_ptr<WebPage> webPage):
            WebPageDecorator(std::move(webPage))
    {}

    void authenticateUser()
    {
        std::cout << "authentification done" << std::endl;
    }
    void display() override
    {
        authenticateUser();
        WebPageDecorator::display();
    }
};

struct AuthorizedWebPage : WebPageDecorator
{
    AuthorizedWebPage(std::unique_ptr<WebPage> webPage):
            WebPageDecorator(std::move(webPage))
    {}

    void authorizedUser()
    {
        std::cout << "authorized done" << std::endl;
    }
    void display() override
    {
        authorizedUser();
        WebPageDecorator::display();
    }
};

int main(int argc, char* argv[])
{
    std::unique_ptr<WebPage> myPage = std::make_unique<BasicWebPage>();

    myPage = std::make_unique<AuthorizedWebPage>(std::move(myPage));
    myPage = std::make_unique<AuthenticatedWebPage>(std::move(myPage));
    myPage->display();
    std::cout << std::endl;
    return 0;
}