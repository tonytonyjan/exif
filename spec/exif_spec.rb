require 'exif'

describe Exif do
  before :all do
    @data = Exif::Data.new(File.expand_path('../sample.jpg', __FILE__))
  end

  it '#dump' do
    # expect { @data.dump }.not_to raise_error
  end

  it 'returns empty hash if none' do
    expect(@data.contents).to  be_a Hash
    expect(@data.contents[:xxx]).to  be_a Hash
  end

  it 'works' do
    expect(@data.contents[:ifd_0]["ImageWidth"]).to eq '4000'
    expect(@data.contents[:gps]["GPSLatitude"]).to eq '24, 10.6817,  0'
  end
end