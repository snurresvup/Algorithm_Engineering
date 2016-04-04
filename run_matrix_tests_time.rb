require 'open3'

runs = 5

Dir["obj/*.o"].each do |obj|
  prog = File.basename(obj, File.extname(obj))
  puts prog
    n = 5000
    output = ""
    while n <= 5500 
      sum = 0
      (1..runs).each do |run|
        res, _ = Open3.capture2("./#{obj}", "#{n}")
        time = res.split(" ")[1]
        sum += time.to_i
      end
      output += "#{n} #{sum/runs}\n"
      n = n+500
      puts "output so far: \n#{output}"
    end
    File.open("data/#{prog}_time_data_large", 'w') do |file| file.write(output) end
end
