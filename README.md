# OpenStackSwiftQtClient
OpenStack Swift Qt  Client
这是一个针对OpenStack Swift的Qt的Client，包括文件的增删查改，bucket的增加和删除，创建删除文件夹，获取文件属性等接口。只需要改mainweight.cpp中的

```
    list_bucket_files(ret_token_message,"photos");
    //upload_file(ret_token_message,"photos","testfolder");
    //get_file_property(ret_token_message,"photos","testfolder/svn使用方法.docx");
    //delete_file(ret_token_message,"photos","testfolder/svn使用方法.docx");
    //download_file(ret_token_message,"photos","testfolder/svn使用方法.docx","我的测试文档.docx","C:/Users/ss/Desktop");
    //make_folder(ret_token_message,"photos","testfolder01");
    //create_bucket(ret_token_message,"bucketname02");
```


代码即可
