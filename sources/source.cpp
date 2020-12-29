#include <FileSystem.hpp>
#include <iostream>
#include <utility>

void MyFilesystem::PrintAllFiles() {
  File tmpFileData;
  if (!boost::filesystem::exists(MyPath)){
    std::cout << "doesn't exist" << std::endl;
  } else{
    if(boost::filesystem::is_regular_file(MyPath)){
      std::cout<< MyPath.filename() << std::endl;
    }else if(boost::filesystem::is_directory(MyPath)){
      for(entry &x : iterator(MyPath)){
        if (boost::filesystem::is_directory(x.path())){
          PrintAllFiles(x.path());
        }
        if (boost::filesystem::is_regular_file(x.path())){
          if(IsFileNameTrue(x.path())){
            std::cout << MyPath.filename().string() << " " <<\
                x.path().filename().string() << std::endl;
            tmpFileData.Broker = MyPath.filename().string();
            string name = x.path().stem().string();
            tmpFileData.Id = name.substr(name.find('_')+1,
                                         name.rfind('_') - name.find('_') - 1);
            tmpFileData.Date = name.substr(name.rfind('_')+1);
            if (IsAccExist(tmpFileData) != -1)
              AccsList[IsAccExist(tmpFileData)].AddFile(tmpFileData);
            else
              AccsList.emplace_back(tmpFileData);
          }
        }
        if (boost::filesystem::is_symlink(x.path())){
          std::cout << "\t" << "Symlink: " << x.path() << std::endl;
        }
      }
    } else if(boost::filesystem::is_symlink(MyPath)){
      std::cout << "Symlink: " << MyPath << std::endl;
      path link = boost::filesystem::read_symlink(MyPath);
      if(boost::filesystem::is_regular_file(link))
        std::cout << link.branch_path().filename().string() <<\
            link.filename().string() << std::endl;
      if(boost::filesystem::is_directory(link))
        PrintAllFiles(link);
    }
  }
  std::cout << std::endl;
}

void MyFilesystem::PrintAllFiles(const path& p) {
  File tmpFileData;
  if (!boost::filesystem::exists(p)){
    std::cout << "doesn't exist" << std::endl;
  } else{
    if(boost::filesystem::is_directory(p)){
      for(entry &x : iterator(p)){
        if (boost::filesystem::is_directory(x.path())){
          PrintAllFiles(x.path());
        }
        if (boost::filesystem::is_regular_file(x.path())){                       //Если файл
          if(IsFileNameTrue(x.path())){
            std::cout << p.filename().string() << " " <<\
                x.path().filename().string() << std::endl;
            tmpFileData.Broker = p.filename().string();
            string name = x.path().stem().string();
            tmpFileData.Id = name.substr(name.find('_')+1,
                                         name.rfind('_') - name.find('_') - 1);
            tmpFileData.Date = name.substr(name.rfind('_')+1);
            if (IsAccExist(tmpFileData) != -1)
              AccsList[IsAccExist(tmpFileData)].AddFile(tmpFileData);
            else
              AccsList.emplace_back(tmpFileData);
          }
        }
        if (boost::filesystem::is_symlink(x.path())){
          std::cout << "Symlink: " << x.path() << std::endl;
        }
      }
    } else if(boost::filesystem::is_symlink(p)){
      std::cout << "Symlink: " << p << std::endl;
      path link = boost::filesystem::read_symlink(p);
      if(boost::filesystem::is_regular_file(link))
        std::cout << link.branch_path().filename().string() <<\
            link.filename().string() << std::endl;
      if(boost::filesystem::is_directory(link))
        PrintAllFiles(link);
    }
  }
}

bool MyFilesystem::IsFileNameTrue(const path& p) {
  string name(p.stem().string());
  if (name.size() < 5)
    return false;
  if (name.substr(name.size()-4) == ".old")
    return false;
  unsigned count = 0;
  for(const char &i : name){
    if (i == '_'){
      ++count;
    }
  }
  if(count!=DASHCOUNT)
    return false;
  string account = name.substr(name.find('_')+1,
                               name.rfind('_') - name.find('_') - 1);
  for(const char &i : account){
    if(!(i >= '0' && i<= '9'))
      return false;
  }
  string date = name.substr(name.rfind('_')+1);
  for(const char &i : date){
    if(!(i >= '0' && i<= '9'))
      return false;
  }
  string temp(date.substr(date.size()-DAYCHARS));
  int day_val = std::stoi(temp);
  if(day_val > MAXDAY)
    return false;
  temp = date.substr(YEARCHARS,MONTHSCHARS);
  if(std::stoi(temp) > MAXMONTH)
    return false;
  return true;
}

int MyFilesystem::IsAccExist(File &f) {
  for(size_t i = 0; i < AccsList.size(); ++i){
    if(AccsList[i] == f)
      return (int)i;
  }
  return -1;
}

void MyFilesystem::PrintTotalInformation() {
  for(size_t i = 0; i<AccsList.size(); ++i){
    std::cout << "broker:" << AccsList[i].GetBroker() << \
        " account:" << AccsList[i].GetId() << \
        " files:" << AccsList[i].GetFiles() << \
        " lastdate:" << AccsList[i].GetData() << std::endl;
  }
}

Account::Account(string broker, string id, int files,
                 string year, string month, string day){
  Broker = std::move(broker);
  Id = std::move(id);
  Files = files;
  Year = std::move(year);
  Month = std::move(month);
  Day = std::move(day);
}

bool Account::operator==(File &Another) {
  if (Broker == Another.Broker && Id == Another.Id)
    return true;
  else
    return false;
}

void Account::AddFile(File &Another) {
  ++Files;
  string fileDay = Another.Date.substr(Another.Date.size()-DAYCHARS);
  string fileMonth = Another.Date.substr(YEARCHARS,MONTHSCHARS);
  string fileYear = Another.Date.substr(0,YEARCHARS);
  if (std::stoi(fileYear) > std::stoi(Year)){
    Year = fileYear;
    Month = fileMonth;
    Day = fileDay;
  } else if (std::stoi(fileMonth) > std::stoi(Month)
             && std::stoi(fileYear) == std::stoi(Year)){
    Year = fileYear;
    Month = fileMonth;
    Day = fileDay;
  } else if (std::stoi(fileDay) > std::stoi(Day)
             && std::stoi(fileMonth) == std::stoi(Month)
             && std::stoi(fileYear) == std::stoi(Year)){
    Year = fileYear;
    Month = fileMonth;
    Day = fileDay;
  }
}
Account::Account(File &f) {
  Broker = f.Broker;
  Id = f.Id;
  Year = f.Date.substr(0,YEARCHARS);
  Month = f.Date.substr(YEARCHARS,MONTHSCHARS);
  Day = f.Date.substr(f.Date.size()-DAYCHARS);
  Files = 1;
}
string Account::GetBroker() { return Broker; }
string Account::GetId() { return Id; }
int Account::GetFiles() { return Files; }
string Account::GetData() { return Year+Month+Day; }
